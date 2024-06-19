#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFFER 256

int main(int argc, char* argv[]) {

size_t buffer=0;
	
if (argc!=3 && argc!=4) {
	printf("Prova a scrivere: ./mycp_sc file1 file2 \n");
	exit(EXIT_FAILURE);
}

if (argc==4) {
	buffer=atoi(argv[3]);
}
else buffer=BUFFER;

//apertura file1
FILE* fd; //descrittore del file1

if (( fd=fopen(argv[1], "r") ) == NULL ) {
	perror("Tentativo di apertura del file");
	exit(EXIT_FAILURE);
}

FILE* fd2; //descrittore del file2

if (( fd2=fopen(argv[2], "w") ) == NULL ) {
	perror("Tentativo di apertura del file");
	exit(EXIT_FAILURE);
}


//lettura file
char* buf=malloc(buffer*sizeof(char));//Creo il Buffer e alloco memoria
size_t lungr; //restituisce un numero>0 se vengono letti byte. -1: errore e 0 se abbiamo raggiunto l'eof
size_t lungw=1; 



while ((lungr=fread(buf, sizeof(char), sizeof(buf), fd))>0) { //lungr restituisce il numero di byte letti! ovviamente se siamo quasi a fine file se ne accorge e ne legge meno

lungw=fwrite(buf, sizeof(char), lungr,fd2); 

	if (lungw<=0) {
		perror("errore fwrite");
		exit(EXIT_FAILURE);	
	}
}



if (lungr==-1) {	
	perror("errore read");
	exit(EXIT_FAILURE);		
}


//chiusura descrittori file
if (fclose(fd)==-1) {
	perror("file1: close");
	exit(EXIT_FAILURE);
}

if (fclose(fd2)==-1) {
	perror("file2: close");
	exit(EXIT_FAILURE);
}


return 0;
}
