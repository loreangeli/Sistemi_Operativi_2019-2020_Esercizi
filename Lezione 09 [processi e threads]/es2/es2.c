/*
Scrivere un programma C con due threads, un produttore (P) ed un consumatore (C). Il thread P genera, uno alla volta, una sequenza di numeri inserendoli in un buffer di una sola posizione condiviso con il thread C. Il thread consumatore estrae i numeri dal buffer e li stampa sullo standard output. Se il buffer e' pieno P attende che C consumi il dato, analogamente se il buffer e' vuoto C attende che P produca un valore da consumare. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h> 
#define N 3 //numero di valori da produrre.

/* implementazione buffer*/

struct buffer {
	int buf; //elemento del buffer
	int empty; //empty=1 buffer vuoto | empty=0 buffer pieno
};

struct buffer b;

// inizializzo buffer 
void inizializza(struct buffer* buf) {
	buf->empty=1;
}
/* inserisce un valore nel buffer 
inserisci restituisce 0 se tutto ok, else 1.
*/
int inserisci(int val, struct buffer* buf) { 
	if (buf->empty==1) {
		buf->empty=0;
		buf->buf=val;
		return 0;
	}
	
	return 1;	
}

/*
estrai svuota la coda
restituisce l'elemento che conteneva altrimenti se vuota restituisce -1
*/
int estrai(struct buffer* buf) {
	if (buf->empty==1)
		return -1;
 
	buf->empty=1;
	return buf->buf; 
}

/* implementazione produttore e consumatore */

pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER; //dichiarazione di variabile mutex lock.
pthread_cond_t cond=PTHREAD_COND_INITIALIZER; //dichiarazione di v.c(variabile di condizione) -->produttore


// produttore 
static void* produttore (void* arg) {	
	int i=0;

	while(i<N) {
		//produci valore casuale x
		int casual=(7+b.buf)%40;
		//acquisisce chiave
		pthread_mutex_lock(&mtx);
		//controllo se buffer pieno
		while (b.empty==0) { //buffer pieno
			pthread_cond_wait(&cond, &mtx);			
		}
		//inserisci x nel buffer
		int ris=inserisci(casual, &b);
		printf("Il produttore ha prodotto: %d \n", b.buf);	
		//risveglia un thread in stato wait
		pthread_cond_signal(&cond);
		//rilascia chiave
		pthread_mutex_unlock(&mtx);

		i++;
	}

	return (void*) 0;
}

// consumatore 
static void* consumatore (void* arg) {
	int j=0;

	while(j<N) { 
		//acquisisce chiave
		pthread_mutex_lock(&mtx);
		//controllo se buffer vuoto
		while (b.empty==1) //buffer vuoto
			pthread_cond_wait(&cond, &mtx);
		//consumo	
		printf("Il consumatore ha consumato: %d \n", b.buf);
		b.empty=1;
		//risveglia un thread in stato wait
		pthread_cond_signal(&cond);	
		//rilascia chiave
		pthread_mutex_unlock(&mtx);

		j++;
	}

	return (void*) 0;
}


int main() {

int err;
pthread_t tidproducer; 
pthread_t tidconsumer;

printf("Esercizio produttori consumatori con %d produttori \n", N);
printf("\n");
//inizializzo buffer
inizializza(&b);

//creo thread produttore P
if ( (err=pthread_create(&tidproducer, NULL, produttore, NULL )) !=0 ) {
	printf("Errore nella creazione del thread Produttore \n");
	return EXIT_FAILURE;
}
//creo thread consumatore C
if ( (err=pthread_create(&tidconsumer, NULL, consumatore, NULL )) != 0){
	printf("Errore nella creazione del thread Consumatore \n");
	return EXIT_FAILURE;
}

//attesa terminazione produttore
pthread_join(tidproducer, NULL); //sospende il processo che invoca il thread tid finchè il thread tid non termina.
//attesa terminazione consumatore
pthread_join(tidconsumer, NULL);


return 0;
}
