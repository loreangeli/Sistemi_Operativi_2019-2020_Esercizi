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
#include <sys/select.h>
#define UNIX_PATH_MAX 108
#define SOCKNAME "./mysock"
#define N 100

int aggiorna(int fd_num, int fd) {
	if (fd_num==fd)
		return fd_num-1;
	
	return fd_num;
}

int main() 
{

	struct sockaddr_un sa;
	strcpy(sa.sun_path, SOCKNAME);
	sa.sun_family=AF_UNIX;

	int fd_sk, fd_c, fd_num, fd;
	unlink(SOCKNAME);
	fd_num=0; //file descriptor max attivo
	char buf[N];
	fd_set set, //insieme dei file descriptor attivi
	rdset; //maschera di bit di appoggio
	int nread; //numero caratteri letti

	//passo 1: creo socket
	if ((fd_sk=socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		if (errno!=0) {
			perror("errore socket");
			exit(EXIT_FAILURE);
		}	
	} 
	//passo 2: bind
	if ( bind(fd_sk,(struct sockaddr *)&sa,sizeof(sa)) == -1 ) {
		if (errno!=0) {
			perror("errore bind");
			exit(EXIT_FAILURE);
		}	
	}
	//passo 3: listen (attivo il socket a poter accettare connessioni da  parte di altri processi)
	if ( listen(fd_sk, SOMAXCONN) == -1) {
		if (errno!=0) {
			perror("errore listen");
			exit(EXIT_FAILURE);
		}	
	}
	
	if (fd_sk > fd_num) fd_num=fd_sk;
	FD_ZERO(&set); //azzero maschera di bit
	FD_SET(fd_sk, &set); //aggiungo il file descriptor alla maschera set
	
	while(1) {
		rdset=set;
		if (select(fd_num+1, &rdset, NULL,NULL,NULL)==-1) { //la select monitora i descrittori file presenti in rdset
		//si  blocca finchè non si ha un fd pronto, all'uscita della select rdset è stato modificato 
		//(contiene gli fd pronti).
			perror("errore select");
			exit(EXIT_FAILURE);
		}	
		else {
			for (fd=0;fd<=fd_num;fd++) { //scorro tutti i file descriptor
				if (FD_ISSET(fd, &rdset)) { //restituisce 1 se il bit del fd presente in rdset vale 1 altrimenti 0
					if (fd==fd_sk) {
						fd_c=accept(fd_sk,NULL,0);
						FD_SET(fd_c, &set);
						if (fd_c > fd_num) fd_num=fd_c;
					}
					else {
						nread=read(fd,buf,N);
						//printf("leggo %d \n",nread);
						if (nread==0) { 
							FD_CLR(fd,&set);
							fd_num=aggiorna(fd_num,fd);
							close(fd);
						}
						else {
							printf("Server legge: %s \n", buf);
							write(fd, "Ricevuto!", 10);
						}
					}
				} 
			}
		} 
	}
		
	return 0;
}

