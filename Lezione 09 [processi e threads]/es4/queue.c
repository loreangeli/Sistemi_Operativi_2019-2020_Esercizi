#include "queue.h"
//CODA LIFO

pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER; //dichiarazione di variabile mutex.

struct Queue initialize(struct Queue coda) {
	pthread_mutex_lock(&lock);

	coda.pos=0;
	coda.coda=malloc(sizeof(char*) * N);

	pthread_mutex_unlock(&lock);
	return coda;
}

struct Queue push(char* stringa, struct Queue coda) {
	pthread_mutex_lock(&lock);
	//coda piena
	if (coda.pos>=100) {
		pthread_mutex_unlock(&lock);
		return coda; 	
	}

	int length=strlen(stringa);
	coda.coda[coda.pos]=malloc(sizeof(char)*(length+1));
	strcpy(coda.coda[coda.pos], stringa);
	coda.pos++;
		
	pthread_mutex_unlock(&lock);
	return coda; //inserimento eseguito con successo
}

char* pop(struct Queue* coda) {
	pthread_mutex_lock(&lock);
	//coda vuota
	if ( coda->pos ==0) {
		pthread_mutex_unlock(&lock);
		return NULL;
	}
	
	char* tmp=malloc(sizeof(char)*100);
	strcpy(tmp, coda->coda[coda->pos-1]);	
	free(coda->coda[coda->pos]);
	coda->pos--;	

	pthread_mutex_unlock(&lock);
	return tmp;
}

int empty(struct Queue coda) {
	pthread_mutex_lock(&lock);

	if (coda.pos==0) {
		pthread_mutex_unlock(&lock);
		return 1;
	}
	
	pthread_mutex_unlock(&lock);
	return 0;
}

void Print(struct Queue coda) {
	for(int i=0;i<coda.pos;i++) {
		printf("%s ", coda.coda[i]);
	}
}


