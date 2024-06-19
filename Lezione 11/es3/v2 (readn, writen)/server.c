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

int aggiorna(int fd_num, int fd) {
	if (fd_num==fd)
		return fd_num-1;
	
	return fd_num;
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

	int fd_sk, fd_c, fd_num, fd;
	//elimina il file "mysocket" se presente
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
		if (select(fd_num+1, &rdset, NULL,NULL,NULL) == -1) { //la select monitora i descrittori file presenti in rdset
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
						//leggo dimensione messaggio
						readn(fd,buf,sizeof(int));
						int dim=atoi(buf);
						//leggo messaggio
						nread=readn(fd,buf,dim);
						if (nread==0) { 
							FD_CLR(fd,&set);
							fd_num=aggiorna(fd_num,fd);
							close(fd);
						}
						else {
							printf("parola ricevuta: %s \n", buf);
							mintomax(buf);

							//gestione messaggio
							int dim=strlen(buf)+1;
							char* buffer=malloc(sizeof(char)*15);
							buffer=itoa(dim,buffer); //converto intero in stringa
							//invio dimensione messaggio
							writen(fd,buffer,(size_t)sizeof(int));
							//invio messaggio
							writen(fd, buf, strlen(buf)+1);
						}
					}
				} 
			}
		} 
	}
		
	return 0;
}

