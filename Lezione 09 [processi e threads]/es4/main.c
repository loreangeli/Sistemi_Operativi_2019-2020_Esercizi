#include <threads.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#include "queue.h"
//code
struct Queue buff1;
struct Queue buff2;
//lista parole token3
char** lista; 
int pos3; //posizione prima cella vuota

FILE* fp; //descrittore file
int eof; //ho letto tutte le righe
int eof2; //ho tokenizzato tutte le righe

//funzione utilità che rimuove dalla stringa \n
char* removecapo(char* stringa) {
	int i=0;
	int lun=strlen(stringa);
	int presente=0; //messo a 1 se c'è \n
	//controlla se c'è il carattere \n
	while (i<lun) {
		if (stringa[i]=='\n')
			presente=1;
		i++;
	}
	if (presente==0) return stringa;

	i=0;
	while (stringa[i]!='\n' && i<lun)
		i++;
	if (stringa[i]=='\n')
		stringa[i]='\0';

	return stringa;
}


/* Thread 1 che si occupa della lettura */
void* Leggi(void* arg) {
	char* res;
	char buffer[100];

	res=fgets(buffer, 100, fp);

	while (res!=NULL) {
		//salvo nella coda 1 la riga	
		buff1=push(buffer, buff1);
		res=fgets(buffer, 100, fp);
	}	
	
	eof=1;	

	return (void*) 0;
}

/* Thread 2 che si occupa della tokenizzazione */

void* Token(void* arg) {
	char* token;	
	char* A;
	char* saveptr1;

	while(1) { 
		
		if (eof==1 && empty(buff1)==1)
			break;

		//punta alla riga	
		A=malloc(sizeof(char)*100);
		A=pop(&buff1);
		
		if (A!=NULL) {
				token = strtok_r(A, " ", &saveptr1);
			//cerca i tokens
			while( token != NULL ) {
					//salvo la parola nella coda 2
					buff2=push(removecapo(token), buff2);
					token = strtok_r(NULL, " ", &saveptr1);
			}			
		}
	
		free(A);
	}	//chiusura while
	
	eof2=1;	

	return (void*) 0;
}

/* Thread 3 che si occupa della scelta delle parole */
void* Scegli (void * arg) {
	int ins=0; 
	char* A;

	while (1) {
		
		if (eof2==1 && empty(buff2)==1)
			break;

		A=malloc(sizeof(char)*100);
		A=pop(&buff2);	

		if (A!=NULL) {
			for(int h=0;h<pos3;h++) {
				if( pos3!=0 && strcmp(A, lista[h])==0 ) //sono uguali
					ins=1;
			}

			if (ins==0)	{		
				lista[pos3]=malloc(sizeof(char)*100);
				strcpy(lista[pos3], A);
				pos3++;
			}
		}

		free(A);
		ins=0;
	}

	//stampo lista
	for(int i=0;i<pos3;i++)
		printf("[%s] \n", lista[i]);

	return (void*) 0;
}

int main(int argc, char** argv) {
//inizialiazza code
buff1=initialize(buff1);
buff2=initialize(buff2);
lista=malloc(sizeof(char*)*100);
pos3=0;
eof=0;
eof2=0;
//inizializza
int status, err;
pthread_t tid1;
pthread_t tid2;
pthread_t tid3;

//indice che indica la prima posizione vuota
if (argc<2) {
	printf("Usa: ./es file.txt \n");
	exit(0);
}

//apertura file
if (( fp = fopen(argv[1],"r")) == NULL) {
	perror("file in apertura");
	exit(EXIT_FAILURE); 
}

//Avvio Thread 1 Leggi
if ( (err=pthread_create(&tid1, NULL, &Leggi, NULL)) != 0 ) { 
	printf("Errore nella creazione del thread Leggi \n");
	exit(-1);
} 


//Avvio Thread 2 Tokenizer

if ( (err=pthread_create(&tid2, NULL, &Token, NULL)) != 0 ) { 
	printf("Errore nella creazione del thread Leggi \n");
	exit(-1);
} 


//Avvio Thread 3 Scegli
if ( (err=pthread_create(&tid3, NULL, &Scegli, NULL)) != 0 ) { 
	printf("Errore nella creazione del thread 3 Scegli \n");
	exit(-1);
} 

//attendo e termino il Thread 1 Leggi
pthread_join(tid1,(void*) &status); //sospende il processo che invoca il thread tid finchè il thread tid non termina.
if (status !=0 ) {
	printf("Errore nella join del thread 1 Leggi \n");
	exit(-1);
}

//attendo e termino il Thread 2 Tokenizer

pthread_join(tid2,(void*) &status); //sospende il processo che invoca il thread tid finchè il thread tid non termina.
if (status !=0 ) {
	printf("Errore nella join del thread 2 Tokenizer \n");
	exit(-1);
}


//attendo e termino il Thread 3 Scegli
pthread_join(tid3,(void*) &status); //sospende il processo che invoca il thread tid finchè il thread tid non termina.
if (status !=0 ) {
	printf("Errore nella join del thread 3 Scegli \n");
	exit(-1);
}


return 0;
}
