/*
Se ci sono N filosofi allora creo un array circolare mod N.
La sequenza di ogni filosofo è -> meditare, cercare di acquisire le bacchette e mangiare. Per poter mangiare ha quindi bisogno di entrambe le bacchette vicine a lui.

Quando il filosofo i acquisisce la chiave controlla se sono disponibili le bacchette i e i-1. Se entrambe lo sono allora posso far mangiare il filosofo altrimenti il thread viene messo nella lista della v.c (variabile di condizione) di wait (facendo la wait ovviamente la chiave viene rilasciata atomicamente). In caso contrario dopo aver atteso che il filosofo abbia terminato di mangiare (utilizzo una sleep random per decidere il tempo che deve mangiare) faccio una signal (la signal non assegna la chiave, semplicemente toglie un processo dalla waiting list e o mette nela lista dei processi pronto. Quindi dopo la signal il processo che chiamato la signal contina a mantenere la chiave) e dopo una release della chiave. 
*/
#include <threads.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#define N 5 //numero di filosofi al tavolo (è possibile cambiarlo)
#define K 100 //numero di volte in cui ogni filosofo esegue il ciclo: medita/cerca di acquisire le bacchette/mangia.

int Bacchette[N]; //questo array serve ad individuare le bacchette occupate. Se Bacchette[i]==-1 bacchetta disponibile.

void inizializzaBacchette(int Bacchette[]) {
	for (int i=0;i<N;i++)
		Bacchette[i]=-1;
	return;
}

pthread_mutex_t locksx=PTHREAD_MUTEX_INITIALIZER; //dichiarazione di variabile mutex.
pthread_mutex_t lockdx=PTHREAD_MUTEX_INITIALIZER; //dichiarazione di variabile mutex.

void* Filosofo(void* arg) {
	struct timespec req, rem;
	unsigned int seed;
	int random;
	int filosofo=*((int*) arg)-1;
	int i=0;
	int left=filosofo % N;
	int right=filosofo-1;
	
	while (i<K) {	
		//random	
		req.tv_sec=0;
		seed=time(NULL);
		random=rand_r( &seed );
		req.tv_nsec=random;	
		if (req.tv_nsec<0)
			req.tv_nsec=req.tv_nsec*(-1);
		//pensa
		printf("Filosofo %d: medita \n", filosofo);
		nanosleep(&req, &rem);

		if (left<right) {
			pthread_mutex_lock(&lockdx);
			pthread_mutex_lock(&locksx);
			printf("Filosofo %d: mangia \n", filosofo);
			//mangia
			seed=time(NULL);
			random=rand_r( &seed );
			req.tv_nsec=random+rem.tv_nsec;	
			if (req.tv_nsec<0)
				req.tv_nsec=req.tv_nsec*(-1);
			nanosleep(&req, NULL);

			pthread_mutex_unlock(&locksx);
			pthread_mutex_unlock(&lockdx);
		} else {
			pthread_mutex_lock(&locksx);
			pthread_mutex_lock(&lockdx);
			//mangia
			seed=time(NULL);
			random=rand_r( &seed );
			req.tv_nsec=random+rem.tv_nsec;	
			if (req.tv_nsec<0)
				req.tv_nsec=req.tv_nsec*(-1);
			nanosleep(&req, NULL);
			pthread_mutex_unlock(&lockdx);
			pthread_mutex_unlock(&locksx);

		}

		i++;
	}
}

int main(int argc, char** argv) {

printf("README: \n1)Ricordati che puoi cambiare il numero dei filosofi (N=%d) \n", N);
printf("2)Puoi anche cambiare il # di volte in cui ogni filosofo fa il ciclo (medita, mangia) (K=%d). \n", K);
printf("Avvio Programma Filosofi con N=%d e K=%d. \n", N, K);
int err, status;
int *arg = malloc(sizeof(*arg));
inizializzaBacchette(Bacchette);

pthread_t tidfilosofo [N]; //creo array di tipo pthread_t

//creazione di N thread Filosofi
for (int i=0;i<N;i++) {
	*arg=i+1;
	printf("Avvio Thread Filosofo %d \n", *arg);
	if ( (err=pthread_create(&tidfilosofo[i], NULL, &Filosofo,arg)) != 0 ) { 
		printf("Errore nella creazione del thread Filosofo %d \n", (i+1));
		exit(-1);
	} 
}

//attendere terminazione di N thread Filosofi
for (int i=0;i<N;i++) {
	pthread_join(tidfilosofo[i],(void*) &status); //sospende il processo che invoca il thread tid finchè il thread tid non termina.

	if (status !=0 ) {
		printf("Errore nella join del thread Filosofo 1 \n");
		exit(-1);
	}
}


return 0;
}
