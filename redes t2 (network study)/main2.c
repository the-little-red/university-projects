#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>
#include "queue.h"

#define maiorp "k"
#define controle "t"
#define TIMEOUT 10
#define MAXHOSTNAME 40
#define MAXMSG 400
#define ZIPPI "10.254.224.23"
#define GOOBER "10.254.224.14"
#define SHIFU "10.254.224.20"
#define CROC "10.254.224.11"

/* ########### ANEL FORMATO ###########
   ####################################
   ######  ZIPPI ---> orval  ########
   ######   ^             |    ########
   ######   |             V    ########
   ######  ?????  <---   CROC  ########
   ####################################
   Casos de emergencia de um server estiver down
   usar o CROC (acesso exclusivo membros do C3SL)
*/
// estrutura da mensagem
typedef struct TipoMensagem{
  struct TipoMensagem *prev;
  struct TipoMensagem *next;
  char destino[40];
  char origem[40];
  char msg[400];
  char proximo[40];
  char prioridade[2];
  char bastao[2];
} TipoMensagem;

TipoMensagem *lista = NULL;
int bastao;
int tempo = 0;
char maiorprio[2];
char meuprio[2];
char proximo[40];

//server, recebe somente a porta
void server (char ip[],char porta[7]){
  int s, t, socketenv, gamb;
  int g = 0;
  int dadoprio, mprio;
	unsigned int i;
  char buf [BUFSIZ + 1];
  char *token;
  char *dados;
  char info[8000];
	struct sockaddr_in sa, isa, se;  /* sa: servidor, isa: cliente */
	struct hostent *hp, *ht;
	char localhost [MAXHOSTNAME];
	char *host, me[MAXHOSTNAME];
  TipoMensagem msg;
  host = ip;
	gethostname (localhost, MAXHOSTNAME);
  gethostname(me,MAXHOSTNAME);
	if ((hp = gethostbyname(localhost)) == NULL){
		puts ("Nao consegui meu proprio IP");
		exit (1);
	}
	sa.sin_port = htons(atoi(porta));
	bcopy ((char *) hp->h_addr, (char *) &sa.sin_addr, hp->h_length);
	sa.sin_family = hp->h_addrtype;
  if ((s = socket(hp->h_addrtype,SOCK_DGRAM,0)) < 0){
    puts ( "Nao consegui abrir o socket" );
  	exit ( 1 );
  }
  if (bind(s, (struct sockaddr *) &sa,sizeof(sa)) < 0){
  	puts ( "Nao consegui fazer o bind" );
  	exit ( 1 );
  }
  //server que vai envia
  if((ht = gethostbyname(host)) == NULL){
    puts("Nao consegui obter endereco IP do servidor.");
    exit(1);
  }
  bcopy((char *)ht->h_addr, (char *)&se.sin_addr, ht->h_length);
  se.sin_family = ht->h_addrtype;
  se.sin_port = htons(atoi(porta));
  if ((socketenv = socket(ht->h_addrtype,SOCK_DGRAM,0)) < 0){
           puts ( "Nao consegui abrir o socket" );
    exit ( 1 );
  }
  while(bastao != 1){
    i = sizeof(isa);

    recvfrom(s, buf, BUFSIZ, 0, (struct sockaddr *) &isa, &i);
    puts("------------------------------------------------");
    puts("------------------------------------------------");
    puts("----------------- ESPERANDO --------------------");
    puts("------------------------------------------------");
    puts("------------------------------------------------");
    token = strtok(buf," ");
    strcpy(msg.destino,token);
    token = strtok(NULL," ");
    strcpy(msg.origem,token);
    token = strtok(NULL," ");

    strcpy(msg.msg,token);
    token = strtok(NULL," ");
    strcpy(msg.prioridade,token);
    token = strtok(NULL," ");

    strcpy(msg.proximo,token);

    token = strtok(NULL," ");

    strcpy(msg.bastao,token);

    if((strcmp(msg.destino,me) == 0) && (strcmp(msg.bastao,"t") != 0)){
      puts("------------------------ || -------------------------");
      printf("| Recebido de %s, mensagem: %s |\n", msg.origem, msg.msg);
      puts("------------------------ || -------------------------");
    }
    if((strcmp(msg.destino,me) == 0) && (strcmp(msg.bastao,"t") == 0) ){
      strcpy(proximo,ip);

      strcpy(msg.proximo,proximo);

      bastao = 1;

      puts("|*********************** || ************************|");
      puts("|***** Bastao recebido, pode enviar mensagens ******|");
      puts("|*********************** || ************************|");
      puts("|***** Enviando mensagem guardada anteriormente ****|");
      puts("|*********************** || ************************|");
    }
    if((strcmp(msg.bastao,"t")) != 0){

      if(strcmp(msg.prioridade,"k")){
        dadoprio = 0;
      }
      if(strcmp(meuprio,"k")){
        mprio = 0;
      }
      if(strcmp(msg.prioridade,"j")){
        dadoprio = 1;
      }
      if(strcmp(meuprio,"j")){
        mprio = 1;
      }
      if(strcmp(msg.prioridade,"f")){
        dadoprio = 2;
      }
      if(strcmp(meuprio,"f")){
        mprio = 2;
      }
      if(strcmp(msg.prioridade,"e")){
        dadoprio = 3;
      }
      if(strcmp(meuprio,"e")){
        mprio = 3;
      }
      if(strcmp(msg.prioridade,"d")){
        dadoprio = 4;
      }
      if(strcmp(meuprio,"d")){
        mprio = 4;
      }
      if(strcmp(msg.prioridade,"c")){
        dadoprio = 5;
      }
      if(strcmp(meuprio,"c")){
        mprio = 5;
      }
      if(strcmp(msg.prioridade,"b")){
        dadoprio = 6;
      }
      if(strcmp(meuprio,"b")){
        mprio = 6;
      }
      if(strcmp(msg.prioridade,"a")){
        dadoprio = 7;
      }
      if(strcmp(meuprio,"a")){
        mprio = 7;
      }
      if(mprio >= dadoprio){
        strcpy(msg.proximo,me);
        strcpy(maiorprio,meuprio);
        strcpy(msg.prioridade,meuprio);
      }else
        strcpy(maiorprio,msg.prioridade);

     }
    puts("ack");
    memcpy(info,msg.destino,MAXHOSTNAME);
    strcat(info," ");
    strcat(info,msg.origem);
    strcat(info," ");
    strcat(info,msg.msg);
    strcat(info," ");
    strcat(info,msg.prioridade);
    strcat(info," ");
    strcat(info,msg.proximo);
    strcat(info," ");
    strcat(info,msg.bastao);
    strcat(info,"\0");
    dados = info;
    if(sendto(socketenv, dados, strlen(dados)+1, 0, (struct sockaddr *) &se, sizeof (se))
      != strlen(dados)+1){
      puts("Nao consegui mandar os dados");
      exit(1);
    }
	}
    close(socketenv);
    close(s);
}
//cliente, recebe ip para enviar, porta e a mensagem
void cliente(char *ip[], char porta[7], TipoMensagem *msg){
    int sockdescr, socketrec, j, bast;
    int enviado = 0;
    int timeout = 0;
    struct timeval tv;
    struct sockaddr_in sa, isa, se;
    struct hostent *hp, *ht;
    char buf[BUFSIZ+1];
    char *token;
    char *host;
    char *dados;
    char hostname[MAXHOSTNAME];
    char info[8000];
    unsigned int i;

    memcpy(info,msg->destino,MAXHOSTNAME);
    strcat(info," ");
    strcat(info,msg->origem);
    strcat(info," ");
    strcat(info,msg->msg);
    strcat(info," ");
    strcat(info,msg->prioridade);
    strcat(info," ");
    strcat(info,msg->proximo);
    strcat(info," ");
    strcat(info,msg->bastao);
    strcat(info,"\0");

    if(strcmp(msg->bastao, controle) != 0){
      bast = 0;
    }else{
      bast = 1;
    }
    host = ip;
    dados =  info;
    if((hp = gethostbyname(host)) == NULL){
      puts("Nao consegui obter endereco IP do servidor.");
      exit(1);
    }
    bcopy((char *)hp->h_addr, (char *)&sa.sin_addr, hp->h_length);
    sa.sin_family = hp->h_addrtype;
    sa.sin_port = htons(atoi(porta));
    if((sockdescr=socket(hp->h_addrtype, SOCK_DGRAM, 0)) < 0) {
      puts("Nao consegui abrir o socket.");
      exit(1);
    }
    //saber quem eu sou, abrir a socket de recebimento
    gethostname(hostname,MAXHOSTNAME);
    if ((ht = gethostbyname(hostname)) == NULL){
  		puts ("Nao consegui meu proprio IP");
  		exit (1);
  	}
  	se.sin_port = htons(atoi(porta));
  	bcopy ((char *) ht->h_addr, (char *) &se.sin_addr, ht->h_length);
  	se.sin_family = ht->h_addrtype;
    if ((socketrec = socket(ht->h_addrtype,SOCK_DGRAM,0)) < 0){
             puts ( "Nao consegui abrir o socket" );
      exit ( 1 );
    }
    if (bind(socketrec, (struct sockaddr *) &se,sizeof(se)) < 0){
  		puts ( "Nao consegui fazer o bind" );
  		exit ( 1 );
  	}
    while(enviado != 1){
      if(sendto(sockdescr, dados, strlen(dados)+1, 0, (struct
  sockaddr *) &sa, sizeof sa) != strlen(dados)+1){
        puts("Nao consegui mandar os dados");
        exit(1);
      }
      tv.tv_sec = TIMEOUT;
      tv.tv_usec = 0;
      if (setsockopt(socketrec, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0){
         perror("Erro no timeout");
         exit(1);
       }
      if (timeout > 10){
        printf("Servidor indisponivel, rede quebrada! saindo...\n");
        exit(1);
      }
      j=sizeof(isa);
      if(recvfrom(socketrec, buf, BUFSIZ, 0, (struct sockaddr *) &isa, &j) < 0){
        printf("Timeout, reenviando mensagem \n");
        timeout++;
        tempo++;
      }else{
        if(bast != 1){
          token = strtok(buf," ");
          strcpy(msg->destino,token);
          token = strtok(NULL," ");
          strcpy(msg->origem,token);
          token = strtok(NULL," ");
          strcpy(msg->msg,token);
          token = strtok(NULL," ");
          strcpy(msg->prioridade,token);
          token = strtok(NULL," ");
          strcpy(msg->proximo,token);
          token = strtok(NULL," ");
          strcpy(msg->bastao,token);
          if(strcmp(hostname,msg->origem) == 0){
            enviado = 1;
            strcpy(maiorprio,msg->prioridade);
            strcpy(meuprio,"k");
            strcpy(proximo,msg->proximo);
          }else{
            printf("Erro desconhecido, mensagem recebida não é deste destino!!\n");
            exit(1);
          }
        }else{
          enviado = 1;
          strcpy(maiorprio,"k");
          strcpy(meuprio,"k");
          strcpy(proximo,msg->proximo);
        }
      }
  }
    close(socketrec);
    close(sockdescr);
}
int main(int argc, char *argv[]){
    int prioridade = 0;
    int fix = 0;
    char count;
		char port[7] = "7171";
    char destino[MAXHOSTNAME];
    char mensagem[4000];
    char hostname[MAXHOSTNAME];
    TipoMensagem *msg;
    TipoMensagem *bastaom;

    strcpy(maiorprio,"k");

    bastao = atoi(argv[1]);//0 não tem bastão, 1 tem bastão

    gethostname(hostname,MAXHOSTNAME);

    bastaom = (TipoMensagem*) malloc(sizeof(TipoMensagem));
    msg = (TipoMensagem*) malloc(sizeof(TipoMensagem));

    if((strcmp(argv[2],ZIPPI)) == 0){
      strcpy(proximo,"zippi");
    }
    if((strcmp(argv[2],SHIFU)) == 0){
      strcpy(proximo,"shifu");
    }
    if((strcmp(argv[2],GOOBER)) == 0){
      strcpy(proximo,"goober");
    }
    if((strcmp(argv[2],CROC)) == 0){
      strcpy(proximo,"croc");
    }  


    while(1){

        puts("------------------------ || -------------------------");
        printf("| Insira sua msg com no max 400 characteres, seguindo o seguinte padrao: \n");
        printf("| Destino: ZIPPI \n| Mensagem: Hello_world \n| Prioridade da mensagem 1\n");
        puts("------------------------ || -------------------------");

        scanf("%s%c%s%c%d",&destino,&count,&mensagem,&count,&prioridade);

        if((strcmp(destino,"zippi")) == 0){
          fix = 1;
    		}
    		if((strcmp(destino,"shifu")) == 0){
          fix = 1;
        }
    		if((strcmp(destino,"goober")) == 0){
          fix = 1;
        }
    		if((strcmp(destino,"croc")) == 0){
          fix = 1;
        }
        if((strcmp(destino,"bowmore")) == 0){
          fix = 1;
        }
        if((strcmp(destino,hostname)) == 0){
          fix = 2;
        }
        if (fix == 0){
          printf("Nao foi possivel localizar servidor destino, encerrando programa\n");
          exit(1);
    		}
        if (fix == 2){
          printf("Mensagem de destino nao pode ser o mesmo que envia! servidor invalido! encerrando programa\n");
          exit(1);
    		}

        switch (prioridade) {
          case 0:
            strcpy(msg->prioridade,"k");
            break;
          case 1:
            strcpy(msg->prioridade,"j");
            break;
          case 2:
            strcpy(msg->prioridade,"f");
            break;
          case 3:
            strcpy(msg->prioridade,"e");
            break;
          case 4:
            strcpy(msg->prioridade,"d");
            break;
          case 5:
            strcpy(msg->prioridade,"c");
            break;
          case 6:
            strcpy(msg->prioridade,"b");
            break;
          case 7:
            strcpy(msg->prioridade,"a");
            break;
          default:
            printf("Prioridade fora dos limites (Min 0, max 7), programa será encerrado\n");
            exit(1);
            break;
        }
        strcpy(meuprio,msg->prioridade);
        strcpy(msg->origem,hostname);
        strcpy(msg->msg,mensagem);
        strcpy(msg->destino,destino);
        strcpy(msg->proximo,proximo);
        strcpy(msg->bastao,"n");
        strcpy(bastaom->origem,hostname);
        strcpy(bastaom->msg,mensagem);
        strcpy(bastaom->prioridade,"k");
        strcpy(bastaom->proximo,proximo);
        strcpy(bastaom->bastao,"t");

        queue_append((queue_t**)&lista,(queue_t*)msg);

        if(wcscmp(maiorprio,msg->prioridade) <= 0){
          strcpy(maiorprio,msg->prioridade);
        }
        if(bastao != 0){
          cliente(argv[2],port,(TipoMensagem*)queue_remove((queue_t**)&lista,(queue_t*)lista->prev));
          tempo++;
          if(tempo >= 5){
            strcpy(bastaom->destino,proximo);
            cliente(argv[2],port,bastaom);
            if((strcmp(bastaom->destino,hostname)) != 0){
              bastao = 0;
            }else{
              bastao = 1;
            }
            tempo = 0;
          }
      }else{
        puts("------------------------ || -------------------------");
        puts("| Nao tenho bastao, modo server |");
        puts("------------------------ || -------------------------");
        server(argv[2],port);
        cliente(argv[2],port,(TipoMensagem*)queue_remove((queue_t**)&lista,(queue_t*)lista->prev));
      }
  }
}
