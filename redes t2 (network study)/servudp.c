/* Prof. Elias P. Duarte Jr.
   Servidor UDP              */

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMFILA      5
#define MAXHOSTNAME 30

main ( int argc, char *argv[] )
  {
	int s, t;
	unsigned int i;
        char buf [BUFSIZ + 1];
	struct sockaddr_in sa, isa;  /* sa: servidor, isa: cliente */
	struct hostent *hp;
	char localhost [MAXHOSTNAME];

        if (argc != 2) {
           puts("Uso correto: servidor <porta>");
           exit(1);
        }

	gethostname (localhost, MAXHOSTNAME);

	if ((hp = gethostbyname( localhost)) == NULL){
		puts ("Nao consegui meu proprio IP");
		exit (1);
	}
	
	sa.sin_port = htons(atoi(argv[1]));

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

       while (1) {
       i = sizeof(isa);
       puts("Vou bloquear esperando mensagem.");
       recvfrom(s, buf, BUFSIZ, 0, (struct sockaddr *) &isa, &i);
       printf("Sou o servidor, recebi a mensagem----> %s\n", buf);
       sendto(s, buf, BUFSIZ, 0, (struct sockaddr *) &isa, i);
	}
}
