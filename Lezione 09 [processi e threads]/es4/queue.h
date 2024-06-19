#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define N 100 //coda lunga 100 elementi!

struct Queue {
	char** coda;
	int pos; //prima posizione libera
};

struct Queue initialize(struct Queue);
struct Queue push(char* stringa, struct Queue);
char* pop (struct Queue*);
int empty (struct Queue); //1 coda vuota else 0
void Print(struct Queue);
