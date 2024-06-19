// Il programma non deve fare uso di variabili globali. 
//Reentrancy: una funzione è rientrante se può essere interrotta in qualsiasi momento durante la sua esecuzione e quindi richiamata in modo sicuro ("rientrata") //prima che le sue precedenti invocazioni completino l'esecuzione.

#include <stdio.h>
#include <stdlib.h>

#if !defined INIT_VALUE
#define INIT_VALUE 0
#endif 

//utilizzo il puntatore val per potermi salvare il contenuto della variabile.
void somma(int x, int* val) {
	*val=*val+x;
}

int main(int argc, char* argv[]) {

int risultato; int risultato2;
int val1=INIT_VALUE; 
int val2=INIT_VALUE;  

int tmp; int tmp2;

if (argc<=1) return -1;

int max=atoi(argv[1]);

for (int i=0;i<max;i++){
	scanf("%d", &tmp);
	somma(tmp, &val1);	
	scanf("%d", &tmp2);
	somma(tmp2, &val2);	
}

printf("Il risultato è: %d \n", val1);
printf("Il risultato è: %d \n", val2);


return 0;
}
