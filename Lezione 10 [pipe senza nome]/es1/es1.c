//RICORDA: Quando creo il processo figlio vengono copiati i valori dei descrittori, delle variabili del processo padre ma da quel momento in poi sono due processi distinti
//M PRODUTTORE 
//N CONSUMATORE
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

struct Stack* stack;

// A structure to represent a stack 
struct Stack { 
    int top; //ultimo elemento occupato 
	 int first; //primo elemento occupato
    unsigned capacity; 
    int* array; 
}; 
  
// function to create a stack of given capacity. It initializes size of 
// stack as 0 
struct Stack* createStack(unsigned capacity) 
{ 
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack)); 
    stack->capacity = capacity*2; 
    stack->top = -1; 
	 stack->first= -1;
    stack->array = (int*)malloc(stack->capacity * sizeof(int)); 
    return stack; 
} 
  
// Stack is full when top is equal to the last index 
int isFull(struct Stack* stack) 
{ 
    return stack->top == stack->capacity - 1; 
} 
  
// Stack is empty when top is equal to -1 
int isEmpty(struct Stack* stack) 
{ 
    return stack->top == -1; 
} 
  
// Function to add an item to stack.  It increases top by 1 
void push(struct Stack* stack, int item) 
{ 
    if (isFull(stack)) {
        return; 
	 }
	 if (stack->top==-1 && stack->first==-1)
		stack->first=0;		
	
	 stack->top=(stack->top)+1;
    stack->array[stack->top] = item; 
    //printf("%d pushed to stack\n", item); 
} 
  
// Function to remove an item from stack.  It decreases top by 1 
int pop(struct Stack* stack) 
{ 
    if (isEmpty(stack)) {
        return INT_MIN; 
	 }
	
	 if (stack->first==stack->top) {
 		int ret=stack->array[stack->first];
	 	stack->first=-1;	
		stack->top=-1;
		return ret;
	 }	


	 int ret=stack->array[stack->first];
    //printf("%d pop to stack\n", stack->array[stack->top]); 
	 stack->first++;
	
    return ret; 
} 
  
// Function to return the top from stack without removing it 
int peek(struct Stack* stack) 
{ 
    if (isEmpty(stack)) 
        return INT_MIN; 
		
	 stack->first++;

    return stack->array[stack->top]; 
} 

void stampa(struct Stack* stack) {
	if (stack->first<0)
		return;

	for (int i=(stack->first);i<(stack->top)+1;i++)
		printf("[%d] ", stack->array[i]);

}


///////////////////////////////////////////////////////////////////////////////
int N, M, K;
int cont;
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER; //lock
pthread_cond_t cond=PTHREAD_COND_INITIALIZER; //var. condizione



//produttore
static void* produttore(void* arg) {
	long* nprod=(long*)arg;
	int message=K/M;
	int resto=K % M;
	
	if (*nprod==0 && resto!=0)	
		message=message+resto;

	for (int i=0;i<message;i++) {
		pthread_mutex_lock(&lock);
		//inserisci messaggio in coda
		push(stack, i);
		printf("Producer %ld pushed to stack <%d> \n", *nprod, i);
		pthread_cond_signal(&cond);	
		pthread_mutex_unlock(&lock);
	}
	return (void*) 0;
}

//produttore di terminazione
static void* produttoreterminazione(void* arg) {
	for (int i=0;i<N;i++) {
		pthread_mutex_lock(&lock);
		//inserisci messaggio in coda
		push(stack, -1);
		pthread_cond_signal(&cond);	
		pthread_mutex_unlock(&lock);
	}
	return (void*) 0;
}

//consumatore
static void* consumatore(void* arg) {
	long contamessaggi=0;
	long* ncons=(long*) arg;

	while(1) {
		pthread_mutex_lock(&lock);	
	
		while(isEmpty(stack)) {
			pthread_cond_wait(&cond, &lock);		
		}

		int x=pop(stack);	
		if (x!=-1) {
			printf("Consumer %ld pop to stack <%d> \n",*ncons, x);
			contamessaggi++;
		}

		if (x==-1) {
		pthread_mutex_unlock(&lock);
			return (void*) contamessaggi;
		}
	
		pthread_mutex_unlock(&lock);
	}
	
	return (void*) contamessaggi;
}

int main (int argc, char** argv) {
	if (argc<4)	{
		printf("Usa: ./es M N K \n");
		exit(0);
	}

	int pid, pid2,  l, err, status; 
	char msg[2]; 
	M=atoi(argv[1]); N=atoi(argv[2]); K=atoi(argv[3]);	
	long* V=malloc(sizeof(long)*M);
	long* L=malloc(sizeof(long)*N);
   stack = createStack(2*K); 

	printf("M produttori | N consumatori | K #totale messaggi \n");
	printf("M: %d N: %d K: %d \n", M, N, K);
	pthread_t* C=malloc(sizeof(pthread_t)*N); //consumatori
	pthread_t* P=malloc(sizeof(pthread_t)*M); //produttori
	pthread_t termine;

	//creo M thread produttori
	for (int i=0;i<M;i++) {
		V[i]=i;
		if ( err=pthread_create(&P[i], NULL, produttore, &V[i]) != 0 ) {
			perror("errore thread produttore");
			return errno;
		}		
	}		

	//creo N thread consumatori
	for (int l=0;l<N;l++) {
		L[l]=l;
		if ( err=pthread_create(&C[l], NULL, consumatore, &L[l]) != 0 ) {
			perror("errore thread consumatore");
			return errno;
		}		
	}		
	
	//attendo M thread produttori
	for (int j=0;j<M;j++) {
		pthread_join(P[j],(void*) &status);
		if (status !=0 ) {
			printf("Errore nella join del thread produttore \n");
			exit(-1);
		}
	}

	//creo thread 1 terminazione (manda un messaggio di terminazione a tutti i thread consumatori facendo una push del messaggio di terminazione, nel nostro esempio impostato a -1)
		if ( err=pthread_create(&termine, NULL, produttoreterminazione, NULL) != 0 ) {
			perror("errore thread terminazione");
			return errno;
		}		
	//attendo thread 1 terminazione	
		pthread_join(termine,(void*) &status);
		if (status !=0 ) {
			printf("Errore nella join del thread terminazione \n");
			exit(-1);
		}

	printf("*----------------------------------------------* \n");
	//attendo N thread consumatori
	for (int i=0;i<N;i++) {
		pthread_join(C[i],(void*) &status);
		printf("Consumer %d, consumed <%d> messages, now it exits \n", i, status);		
	}


return 0;
}
