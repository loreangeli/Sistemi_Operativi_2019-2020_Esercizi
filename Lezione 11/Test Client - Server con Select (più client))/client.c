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

int main() 
{

	struct sockaddr_un sa;
	strcpy(sa.sun_path, SOCKNAME);
	sa.sun_family=AF_UNIX;
	
	int fd_skt;
	char buf[N];
	char leggi[50];
	
	//passo 1: creo socket
	if ((fd_skt=socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		if (errno!=0) {
			perror("errore socket");
			exit(EXIT_FAILURE);
		}	
	} 
	//passo 2: connect (il client si connette tramite socket al socket specificato dal server, nel nostro caso "./mysock")
	while( connect(fd_skt,(struct sockaddr*)&sa,sizeof(sa)) == -1) {
		if (errno == ENOENT) //controlla se il socket è stato creato
			sleep(1);
		else exit(EXIT_FAILURE);
	}

	printf("inserisci operazione: ");
	scanf("%s",leggi);
	while(strcmp(leggi,"quit")!=0) {
		//scrivo nel descrittore restituito dal socket creato che ho associato al server.
		write(fd_skt, leggi, strlen(leggi)+1);
		read(fd_skt,buf,N);
		printf("Client got: %s\n",buf);
		printf("inserisci operazione: ");
		scanf("%s",leggi);
	}

close(fd_skt);

return 0;
}



