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

void cleanup() {
	unlink(SOCKNAME);
}

void reverse(char *s)
{
   int c, i, j;

   for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
      c = s[i];
      s[i] = s[j];
      s[j] = c;
   }
}

char *itoa(int n, char *s)
{
   int i, sign;

   if ((sign = n) < 0)   /* tiene traccia del segno */
      n = -n;   /* rende n positivo */
   i = 0;
   do {   /* genera le cifre nell'ordine inverso */
      s[i++] = n % 10 + '0';   /* estrae la cifra seguente */
   } while ((n /= 10) > 0);   /* elimina la cifra da n */
   if (sign < 0)
      s[i++] = '-';
   s[i] = '\0';
   reverse(s);

   return s;
}

static inline int readn(long fd, void *buf, size_t size) {
	size_t left=size; //numero di byte da leggere
	int r;
	char *bufptr=(char*) buf;
	while (left>0) {
		if ((r=read((int)fd, bufptr,left)) == -1 ) {
			if (errno == EINTR) continue;
			return -1;
		}
		if (r == 0) return 0; //gestione chiusura socket
		left-=r;
		bufptr+=r;
	}
	return size;
}

static inline int writen(long fd, void *buf, size_t size) {
	size_t left=size;
	int r;
	char *bufptr=(char*) buf;
	while (left>0) {
		if ((r=write((int)fd, bufptr,left)) == -1 ) {
			if (errno == EINTR) continue;
			return -1;
		}
		if (r == 0) return 0; //gestione chiusura socket
		left-=r;
		bufptr+=r;
	}
	return 1;
}

void mintomax(char* stringa ) {
    int dim=strlen(stringa);

    for (int i=0;i<dim;i++) {
        if (stringa[i] != ' ') {
            if ( (stringa[i] >= 'A') && (stringa[i] <= 'Z') ) {//controllo che il carattere non sia già maiuscolo
                //non fare niente
            }
            else {
                stringa[i]=stringa[i]-32;
            }
        }
	}
}

static void* spawnserver(void* tid) {
	long* fd_c=(long*)tid;
	char buf[30];

	while (1) {

		//leggo dimensione messaggio
		readn(*fd_c,buf,sizeof(int));
		int dim=atoi(buf); //converte stringa in intero
		
		if (strcmp(buf,"-1")==0) {
			return (void*) 0;
			close(*fd_c);
		}

		//leggo messaggio
		readn(*fd_c,buf,dim);
		printf("messaggio ricevuto: %s \n", buf);
		mintomax(buf);

		/* GESTIONE MESSAGGIO */
		dim=strlen(buf)+1;
		char* buffer=malloc(sizeof(char)*dim);
		buffer=itoa(dim,buffer); //converto intero in stringa
		//invio dimensione messaggio
		writen(*fd_c,buffer,(size_t)sizeof(int));
		//invio messaggio
		writen(*fd_c,buf,dim);
	}
	return (void*) 0;
}

static void spawn(long fd){
	pthread_t tid;
	pthread_attr_t attr;
	long* _fd=malloc(sizeof(long));
	_fd[0]=fd;
	/* inizializza attr */
	pthread_attr_init(&attr);
	/* imposta il detach state */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&tid, &attr, &spawnserver, &_fd[0]); /* passa attr per impostare il detach state sul thread creato */


}


int main() {
	cleanup();
	atexit(cleanup);

	int fd_skt;
	long fd_c;
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

	while (1) {
		//passo 4: accept (il processo si mette in attesa che qualcuno si connetta al socket, quando si connette qualcuno la accept ritorna il file descriptor del nuovo socket che sarà usato nella comunicazione)
		if ( (fd_c=accept(fd_skt,NULL,0)) ==-1 ) {
			if (errno!=0) {
				perror("errore accept");
				exit(EXIT_FAILURE);
			}		
		}
		
		spawn(fd_c);
		
	} //endwhile

return 0;
}