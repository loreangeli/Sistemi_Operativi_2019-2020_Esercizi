#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <time.h> 
#include <sys/socket.h>
#include <sys/un.h>
#define UNIX_PATH_MAX 108
#define SOCKNAME "./mysock"
#define N 100

int main() {
	int fd_skt, fd_c;
	char buf[N];
	struct sockaddr_un sa;
	strncpy(sa.sun_path, SOCKNAME, UNIX_PATH_MAX);
	sa.sun_family=AF_UNIX;
	//passo 1: creo socket
	if ((fd_skt=socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		if (errno!=0) {
			perror("errore socket");
			exit(EXIT_FAILURE);
		}	
	} 
	//passo 2: bind
	if ( bind(fd_skt,(struct sockaddr *)&sa,sizeof(sa)) == -1 ) {
		if (errno!=0) {
			perror("errore bind");
			exit(EXIT_FAILURE);
		}	
	}
	//passo 3: listen (attivo il socket a poter accettare connessioni da  parte di altri processi)
	if ( listen(fd_skt, SOMAXCONN) == -1) {
		if (errno!=0) {
			perror("errore listen");
			exit(EXIT_FAILURE);
		}	
	}
	//passo 4: accept (il processo si mette in attesa che qualcuno si connetta al socket, quando si connette qualcuno la accept ritorna il file descriptor del nuovo socket che sarà usato nella comunicazione)
	if ( (fd_c=accept(fd_skt,NULL,0)) ==-1 ) {
		if (errno!=0) {
			perror("errore accept");
			exit(EXIT_FAILURE);
		}		
	}
	//leggi dal descrittore del client
	read(fd_c,buf,N);
	printf("Server got: %s\n",buf);
	write(fd_c,"Ciao Client",12);
	close(fd_skt);	
	close(fd_c);
	exit(EXIT_SUCCESS);



return 0;
}
