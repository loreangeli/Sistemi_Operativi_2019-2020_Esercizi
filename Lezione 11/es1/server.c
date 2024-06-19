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

static inline void closechannel(int channel[2]) {
	close(channel[0]);
	close(channel[1]);
}


int main() {
	int fd_skt, fd_c;
	char buf[N];
	struct sockaddr_un sa;
	strncpy(sa.sun_path, SOCKNAME, UNIX_PATH_MAX);
	sa.sun_family=AF_UNIX;
	//passo 1: creo socket
	if ((fd_skt=socket(AF_UNIX, SOCK_STREAM, 0)) != 0) {
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

while(1) {
	//leggi dal descrittore del client
	if (read(fd_c,buf,N)==-1) {
		perror("errore read \n");
	}

	if (strcmp(buf,"quit")==0) {
		printf("connection done \n");
		fflush(stdout);
		close(fd_c); //chiudo il descrittore di comunicazione	
		//attendo nuova connessione
		if ( (fd_c=accept(fd_skt,NULL,0)) ==-1 ) {
				if (errno!=0) {
					perror("errore accept");
					exit(EXIT_FAILURE);
				}		
		}
		if (read(fd_c,buf,N)==-1) {
			perror("errore read");
		}
	}

	//elabora calcolo con bc 
	char* arg1=malloc(sizeof(char)*(strlen(buf)+1));
	strcpy(arg1,buf);
	char tmp[20];

	
	int channel[2];
	int channel2[2];
	if (pipe(channel)<0) {
		perror("pipe");
		return errno;
	}
	if (pipe(channel2)<0) {
		perror("pipe");
		return errno;
	}

	pid_t pid1, pid2;
	switch(pid1=fork()) {
		case 0: { //primo figlio, eseguirà echo che scriverebbe nello stdout
			if (dup2(channel[1],1)<0) { //faccio si che stdout non scriva sulla stdout ma sulla pipe
				perror("dup2");
				closechannel(channel);
				return errno;
			}
			
			closechannel(channel);
			closechannel(channel2);

			execlp("/bin/echo", "echo", arg1, NULL);
			perror("execln");
		}
		case -1: {
			perror("forking primo figlio");		
			return errno;
		}
		default:; //sono il padre
	}

	switch(pid2=fork()) {
		case 0: { //secondo figlio eseguirà bc
			if (dup2(channel[0],0)<0) { //da ora in poi chiunque legga dallo stdin legge da channel[0]
				perror("dup2");
				closechannel(channel);
				return errno;
			}
		
			if (dup2(channel2[1],1)<0) { //da ora in poi chiunque legga dallo stdin legge da channel[0]
				perror("dup2");
				closechannel(channel2);
				return errno;
			}

			closechannel(channel);
			closechannel(channel2);
			execlp("/usr/bin/bc", "bc", NULL);	
			perror("execlp");		
			return errno;
		}
		case -1: {
			perror("forking secondo figlio");	
			return errno;
		}
		default:; //padre
	}

	closechannel(channel);
	close(channel2[1]);
	int k=read(channel2[0],tmp,20); //k rappresenta il numero di byte letti. 
	closechannel(channel2);
	
	char risultato[10];
	
	for(int i=0;i<k;i++)
		risultato[i]=tmp[i];

	risultato[k]='\n';

	if (waitpid(pid1, NULL, 0)<0) { perror("waitpid pid1"); return -1;}
	if (waitpid(pid2, NULL, 0)<0) { perror("waitpid pid2"); return -1;}

	//fine elaborazione calcolo

	if (write(fd_c,risultato,strlen(risultato)+1)==-1) {
		perror("errore read");
	}
}


close(fd_skt);	

return 0;
}
