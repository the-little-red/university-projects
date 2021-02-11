#include "main.h"

void client_loop (int socket) {

	printf("-------------------------------------\n");
	printf("client: rodando em modo cliente\n");
	printf("client: escutando a interface %s\n", TRANSFER_DEVICE_CLIENT);
	printf("-------------------------------------\n");

  int j,i = 0;
  char c;
  char *option = (char*) calloc (sizeof(char**), 300);
  char **opt = (char**) calloc (sizeof(char**), 20);
	client_cc ("..");
	printf("\r \n");
	while (1) {

		if (DEBUG)
			printf("debug: proximo pacote %d\n", _seq);

		i = 0;
		printf ("$ ");
		while ((c = getchar ()) != '\n')
			option[i++] = c;
		option[i] = '\0';

		i = 0;
		opt[i] = strtok (option, " ");
		while ((opt[++i] = strtok (NULL, " ")));

		if (strcmp(opt[0], "exit") == 0)
			break;

		else if (strcmp(opt[0], "cd") == 0)
			client_cd (socket, opt[1]);

		else if (strcmp(opt[0], "ls") == 0){
			printf("-------------------------------------- \n");
			printf("---------- / - remoto - /------------- \n");
			client_ls (socket, opt[1]);
			printf("---------- / - remoto - /------------- \n");
			printf("-------------------------------------- \n");
		}

		else if (strcmp(opt[0], "put") == 0)
			file_send (socket, opt[1]);

		else if (strcmp(opt[0], "get") == 0)
			client_get (socket, opt[1]);

    else if (strcmp(opt[0], "cc") == 0)
      client_cc (opt[1]);

    else if (strcmp(opt[0], "ll") == 0){
		  client_ll ();
		}

		else if (strcmp(opt[0],"putb") == 0)
     for(j=1; j<i; j++){
				file_send (socket, opt[j]);
		 }
		else if (strcmp(opt[0], "getb") == 0){
		 for(j=1; j<i; j++){
				client_get (socket, opt[j]);
				printf(" \n");
			}
		}
		else
			client_help ();

		if (DEBUG)
			printf("debug: esperando comando\n");

	}
}

void client_help () {
		printf("help: comandos disponiveis:\n");
		printf("  cd <diretorio> (remoto)\n");
		printf("  cc <diretorio> (local)\n");
		printf("  ls (remoto)\n");
		printf("  ll (local)\n");
		printf("  put <arquivo>\n");
		printf("  putb <arquivo1> <arquivo2> <..> (put bloco)\n");
		printf("  get <arquivo>\n");
		printf("  getb <arquivo1> <arquivo2> <..> (get bloco)\n");
		printf("  help\n");
		printf("  exit\n");
}

void client_cd (int socket, char *folder) {

	if (!folder) {
		printf("cd: forneca o nome de um diretorio\n");
		return;
	}

	packet p = packet_mount (_seq, TYPE_CD,
		(unsigned char*) folder, strlen (folder));

	if (packet_send (socket, p))
		p = packet_recv (socket);

	if (p->type == TYPE_SHOW)
		printf("cd remoto: diretorio %s\n", folder);
	else if (p->type == TYPE_ERROR)
		printf("cd remoto: diretorio nao disponivel\n");

}

void client_ls (int socket, char *opt) {

    int optlen = (opt) ? strlen(opt) : 0;
    packet p = packet_mount (_seq, TYPE_LS, (unsigned char*) opt, optlen);
	packet_send (socket, p);
	p = packet_recv (socket);

	if (p->type == TYPE_SHOW)
		printf("%s\n", p->data);

}

void client_get (int socket, char *file) {

	packet p = packet_mount (_seq, TYPE_GET,
		(unsigned char*) file, strlen(file));

	if (packet_send (socket, p))
		file_recv (socket, file);

}

void client_ll (void) {

	struct dirent *dp;

	char cwd[1024];
  char *out = malloc (1);
	getcwd (cwd, sizeof(cwd));

	DIR *dir = opendir (cwd);
	while ((dp = readdir (dir)) != NULL) {
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
			out = realloc (out, sizeof(char) * (strlen (out) + strlen (dp->d_name)));
			out = strcat (out, dp->d_name);
            sprintf (out, "%s\n", out);
		}
	}
	closedir(dir);
	out[strlen (out)] = '\0';
		printf("------------------------------------- \n");
		printf("---------- / - local - /------------- \n");
    printf ("%s				", out);
		printf("\n");
		printf("---------- / - local - /------------- \n");
		printf("------------------------------------- \n");
    free (out);

}

void client_cc (char* dir) {

	char cwd[1024];

	if ((chdir ((char*) dir)) == 0) {
		getcwd (cwd, sizeof(cwd));
		printf ("cd local: diretorio: %s\n", dir);
	} else {
			printf ("cd local: diretorio invalido! \n");
	}

}
