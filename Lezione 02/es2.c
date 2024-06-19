#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define N 1000
#define K 10

int main() {

int* a= malloc(sizeof(int)*K);
unsigned int seed;
int tmp=0;
short int tme= (short int)time(0);


for (int i=0;i<N;i++) {
	tmp=tme+rand_r (&seed);
	tmp=tmp%K;
	a[tmp]++;
}


for (int i=0;i<K;i++) {
	double ris= (double)a[i]/K;
	printf("[%d]: %.2f", i, ris );
	printf("%% \n");
}




    return 0;
}
