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
	int fd_skt;
	char buf[N];
	struct sockaddr_un sa;
	strncpy(sa.sun_path, SOCKNAME, UNIX_PATH_MAX);
	sa.sun_family=AF_UNIX;
	
	
	//passo 1: creo socket
	fd_skt=socket(AF_UNIX,SOCK_STREAM,0);
	//passo 2: connect (il client si connette tramite socket al socket specificato dal server, nel nostro caso "./mysock")
	while( connect(fd_skt,(struct sockaddr*)&sa,sizeof(sa)) == -1) {
		if (errno == ENOENT) //controlla se il socket è stato creato
			sleep(1);
		else exit(EXIT_FAILURE);
	}
	
	//scrivo nel descrittore restituito dal socket creato che ho associato al server.
char stringa[10];
printf("Scrivi operazione: ");
scanf("%s", stringa);

while ( strcmp(stringa,"quit") !=0 ) {
	write(fd_skt, stringa, strlen(stringa)+1);
	read(fd_skt,buf,N);
	printf("result: %s",buf);
	printf("Scrivi operazione: ");
	scanf("%s", stringa);
}

write(fd_skt, "quit", 5);
close(fd_skt);
exit(EXIT_SUCCESS);

return 0;
}
