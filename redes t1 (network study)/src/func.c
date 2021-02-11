#include "main.h"

int _seq = 1;

/* --- FUNÇÕES QUE LIDAM COM SOCKETS! --- */

void socket_help () {
	printf("modo de lançamento: main [ --client | --server ]\n");
}

int socket_create (const char *device) {
	int soquete;
  struct ifreq ir;
  struct sockaddr_ll endereco;
  struct packet_mreq mr;

  soquete = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));  	/*cria socket*/
  if (soquete == -1) {
    printf("Erro no Socket\n");
    exit(-1);
  }

  memset(&ir, 0, sizeof(struct ifreq));  	/*dispositivo eth0*/
  memcpy(ir.ifr_name, device, sizeof(device));
  if (ioctl(soquete, SIOCGIFINDEX, &ir) == -1) {
    printf("Erro no ioctl\n");
    exit(-1);
  }


  memset(&endereco, 0, sizeof(endereco)); 	/*IP do dispositivo*/
  endereco.sll_family = AF_PACKET;
  endereco.sll_protocol = htons(ETH_P_ALL);
  endereco.sll_ifindex = ir.ifr_ifindex;
  if (bind(soquete, (struct sockaddr *)&endereco, sizeof(endereco)) == -1) {
    printf("Erro no bind\n");
    exit(-1);
  }


  memset(&mr, 0, sizeof(mr));          /*Modo Promiscuo*/
  mr.mr_ifindex = ir.ifr_ifindex;
  mr.mr_type = PACKET_MR_PROMISC;
  if (setsockopt(soquete, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1)	{
    printf("Erro ao fazer setsockopt\n");
    exit(-1);
  }

  return soquete;
}

/* --- FUNÇÕES QUE LIDAM COM OS PACOTES --- */
packet packet_mount (int seq, int type, unsigned char *data, int data_size) {

	packet p = malloc (PACKET_MAXSIZE);
    p->data = malloc (data_size);

	p->init = PACKET_INIT;
	p->size = PACKET_MINSIZE + data_size;
	p->seq = seq;
	p->type = type;
	p->data = data;
	p->par = packet_parity (p);
  //p->par = packet_crc(p->data);
	return p;
}

unsigned char *packet_raw (packet p) {

	int i;
	unsigned char *r = malloc ((p->size + 5) * sizeof (unsigned char));
	r[0] = p->init;
	r[1] = p->size;
	r[2] = p->seq;
	r[3] = p->type;
	for (i = 0; i <= p->size - 3; i++)
		r[i + 4] = p->data[i];
	r[i + 4] = p->par;
	i = 0;

	return r;
}

packet packet_unraw (unsigned char* r) {

	int i;
	packet p = malloc (r[1] - 2);

	p->init = r[0];
	p->size = r[1];
	p->seq = r[2];
	p->type = r[3];
	p->data = malloc (p->size - 2);
    for (i = 0; i <= p->size - 3; i++)
		p->data[i] = r[i + 4];
    p->par = packet_parity (p);
    //p->par = packet_crc(p->data);
	return p;
}


int packet_crc (unsigned char *message){

   int i, j;
   unsigned int byte, crc, mask;
   //unsigned char *r = packet_raw (p);

   i = 0;
   crc = 0xF;
   while (message != 0) {
      byte = message[i];            // Pega proximo byte
      crc = crc ^ byte;
      for (j = 7; j >= 0; j--) {    // Repete por 8 vs
         mask = -(crc & 1);
         crc = (crc >> 1) ^ (0x80F & mask);
      }
      i = i + 1;
   }
   return crc;
}
int packet_parity (packet p) {

    int i = 0;
    int par = 0;
    unsigned char *r = packet_raw (p);

    for (i = 0; i < r[1]; i++)
		par = par ^ r[i];

    return par;
}

int packet_send (int socket, packet p) {

	int i = 0;
	long int random;

	int status;
	struct pollfd pfd;
	pfd.fd = socket;
	pfd.events = POLLIN;

	unsigned char* q = packet_raw (p);
	packet r = malloc (sizeof(char) * PACKET_MAXSIZE);

	srand(time(0));
	while (i++ < PACKET_TRIES) {

		random = (int) pow (2, i) <= 1024 ? (int) pow (2, i) : 1024;
		random = rand() % (random + 1);

		if ((send (socket, q, PACKET_MAXSIZE, 0)) == -1) {
			printf("erro: send: %s\n", strerror(errno));
			usleep (random * PACKET_TIME);
			continue;
		}

		if (DEBUG && p->type != TYPE_ACK) {
			printf("debug: enviada tentativa %d\t", i);
			debug_packet (p);
		}

		if (p->type == TYPE_ACK)
			packet_seq ();

		if (p->type == TYPE_ACK || p->type == TYPE_NACK)
			return 1;

		if ((status = poll (&pfd, 1, (int) random)) < 0 ) {
			printf("erro: poll: %s\n", strerror(errno));
			usleep (random * PACKET_TIME);
			continue;
		}

		if ((r = packet_recv(socket)) == 0) {
			printf("erro: packet_recv: packet_recv: erro no recebimento");
			usleep (random * PACKET_TIME);
			continue;
		}

		if (r->type == TYPE_NACK) {
			printf("erro: packet_recv: nack retornado\n");
			i = 0;
			continue;
		}

		return 1;
	}

	printf("erro: packet_send: falha na conexao\n");
	return 0;
}

packet packet_recv (int socket) {

    unsigned char *r = malloc (PACKET_MAXSIZE);
    packet p = malloc (PACKET_MAXSIZE);

    while (1) {

		if (DEBUG)
			printf("debug: aguardando pacote %d\n", _seq);

		if ((recv (socket, r, PACKET_MAXSIZE, 0)) < 0) {
			printf("erro: recv: %s\n", strerror(errno));
			continue;
		}

		p = packet_unraw (r);

		if (p->init != PACKET_INIT)
			continue;

		if (p->seq != _seq)
			continue;

          // if(packet_crc(p->data) != p->par)
          //       continue;
        if (packet_parity(p) != p->par)
            continue;

		if (p->type == TYPE_ACK) {
			packet_seq ();
			return p;
		}

		if (DEBUG) {
			printf("debug: mensagem recebida\t");
			debug_packet (p);
		}

		if (p->type != TYPE_ACK && p->type != TYPE_NACK) {
			packet q = packet_mount (_seq, TYPE_ACK, NULL, 0);
			if (packet_send (socket, q))
				return p;
		}

	}
}

int packet_seq () {

	if (_seq < PACKET_SEQ)
		++_seq;
	else
		_seq = 1;

	return _seq;
}

/* --- FUNÇÕES QUE LIDAM COM ARQUIVOS  --- */

void file_send (int socket, char *file) {

	char animation = 'O';
    unsigned char* buffer = malloc (PACKET_DATA);
    packet p;

    FILE *fp;
    if ((fp = fopen (file, "r")) == NULL) {
        printf("file: arquivo inexistente\n");
        return;
    }

	fseek (fp, 0, SEEK_END);
	long int filesize = ftell(fp);
	int bufflen = 1;
	rewind (fp);

	p = packet_mount (_seq, TYPE_PUT, (unsigned char*) file, strlen (file));
	if (!packet_send (socket, p))
		return;

	while (1) {

		bufflen = filesize - ftell(fp);

		if (bufflen <= 0)
			break;

		if (bufflen > PACKET_DATA)
			bufflen = PACKET_DATA;
		else
			buffer[bufflen] = '\0';

		fread (buffer, 1, bufflen, fp);

		p = packet_mount (_seq, TYPE_DATA, buffer, bufflen);
		if (!packet_send (socket, p))
			return;

		animation = ext_progress (animation);
		if (!DEBUG)
			printf ("\rfile: enviando %s %3ld%% %c",
				file, ftell(fp) * 100 / filesize, animation);

	}

	fclose (fp);

	char* filesize_uc = malloc (PACKET_DATA);
	sprintf (filesize_uc, "%ld", filesize);

	p = packet_mount (_seq, TYPE_END,
		(unsigned char*) filesize_uc, strlen (filesize_uc));
	if (!packet_send (socket, p))
		return;

	if (!DEBUG)
		printf ("\n");

}

void file_recv (int socket, char *file) {

	long int filesize;
	packet p = packet_mount(_seq, 0, NULL, 0);
	FILE* fp;

	//Evita que um arquivo seja sobrescrito em testes locais
    //sprintf ((char*) file, "%s.tmp", file);

    if ((fp = fopen ((char*) file, "w")) == NULL) {
        printf ("put: nao foi possivel criar arquivo\n");
        return;
    }

    printf ("put: recebendo arquivo: %s\n", file);

	while (1) {
		p = packet_recv (socket);

		if (p->type == TYPE_DATA)
			fwrite (p->data, 1, p->size - 2, fp);

		else if (p->type == TYPE_END) {
			filesize = atol ((char *) p->data);
			break;
		}
	}

    fseek (fp, 0, SEEK_END);
    if (filesize != ftell(fp))
		printf ("put: arquivos com tamanhos diferentes\n");
	else
		printf ("put: transferencia finalizada\n");

	fclose (fp);

}

/* --- FUNÇÕES AUXILIARES --- */

char ext_progress (char c) {
	if (c == 'O')
		return 'o';
	else if (c == 'o')
		return '.';
	else
		return 'O';
}

void ext_cdtousr () {
	int i;
	char dir[1024];

	ext_getexename (dir, sizeof(dir));
	for (i = strlen(dir); dir[i] != '/'; i--);
	dir[i] = '\0';

	char *cwd = malloc (snprintf(NULL, 0, "%s%s", dir, "../usr/") + 1);
	sprintf(cwd, "%s%s", dir, "/../usr/");
	chdir(cwd);
	getcwd (dir, sizeof(dir));

	printf ("ext: diretorio %s\n", dir);
}

char* ext_getexename(char* buf, size_t size) {
    char linkname[64]; // /proc/<pid>/exe
    pid_t pid;
    int ret;

    // Pega o pid e constroi o nome do link em proc/
    pid = getpid();


    if (snprintf(linkname, sizeof(linkname), "/proc/%i/exe", pid) < 0)
        {
					//caso dê muito ruim, aborte
        	abort();
        }


    // Le o link simbolico
    ret = readlink(linkname, buf, size);

    // Caso de erro, quem chamo que vai resolver
    if (ret == -1)
        return NULL;

    // Reporta buffer insuficiente
    if (ret >= size)
        {
        errno = ERANGE;
        return NULL;
        }

    // Null terminator
    buf[ret] = 0;

    return buf;
}

/* --- FUNÇÕES PARA DEBUG --- */

void debug_raw (unsigned char* r) {
	int i = 0;
	printf ("r[0]\tr[1]\tr[2]\tr[3]\n");
	printf ("%x\t%x\t%x\t%c\n", r[0], r[1], r[2], r[3]);

	printf ("r[4]: ");
	for (i = 0; i <= r[1] - 3; i++);
		printf ("%c", r[i + 4]);
	printf ("\n");

	printf ("r[%d]: %x\n", i + 4, r[i + 4]);
}

void debug_packet (packet p) {

	int i;
	printf ("%2x | %3d | %d | %d | %c",
        p->init, p->size, p->seq, p->par, p->type);

	if (DEBUG > 1) {
		printf ("\n--------------\n");
		for (i = 0; i < p->size - 2; i++)
			printf ("%x", p->data[i]);
		printf ("\n--------------\n");
	}
}
