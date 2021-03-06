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

void cleanup() {
	unlink(SOCKNAME);
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
		else {
			perror("errore connect");
			exit(EXIT_FAILURE);
		}
	}

	printf("inserisci operazione: ");
	scanf("%s",leggi);
	printf("ho letto %s \n", leggi);
	while(strcmp(leggi,"quit")!=0) {
		//scrivo nel descrittore restituito dal socket creato che ho associato al server.
		//gestione messaggio
		int dim=strlen(leggi)+1;
		char* buffer=malloc(sizeof(char)*15);
		buffer=itoa(dim,buffer); //converto intero in stringa
		//invio dimensione messaggio
		writen(fd_skt,buffer,(size_t)sizeof(int));
		//invio messaggio 
		writen(fd_skt, leggi, strlen(leggi)+1);
		//leggo dimensione messaggio
		readn(fd_skt,buf,sizeof(int));
		dim=atoi(buf);
		//leggo messaggio
		readn(fd_skt,buf,dim);
		printf("%s\n",buf);
		printf("inserisci operazione: ");
		scanf("%s",leggi);
	}

close(fd_skt);

return 0;
}



