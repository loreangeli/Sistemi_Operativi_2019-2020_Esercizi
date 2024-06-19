#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


void crea_processo(int i, int N) {
	int j=0;
	int pid;
	if (i<N)
		pid=fork();
	if (i>=N)
		exit(0);
	if (i<N && pid==0) {
		for(j=i;j<N-1;j++)
			printf("-");

		if(i==N-1) {
			printf("%d: sono l'ultimo discendente \n", getpid());
		}
		else	printf(" %d: creo un processo figlio \n", getpid());
		//sleep(2);
		crea_processo(i+1,N); //sono il figlio
	}
	else {
		waitpid(pid,NULL,0); //sono il padre
		j=0;
		for(j=i;j<N-1;j++)
			printf("-");
		printf("%d: terminato con successo \n", pid);
	}

	return;
}


int main(int argc, char *argv[]) {
int N=atoi(argv[1]);
int index=0; //contatore che corrisponde al numero di processi eseguiti
int pid=getpid();

//print_creation
for(int i=0;i<N;i++)
	printf("-");
printf(" %d: creo processo figlio \n", getpid());

crea_processo(index, N);

if (pid==getpid()) { //sono il processo main che ha creato tutti i processi :)
	for(int i=0;i<N;i++)
	printf("-");
printf(" %d: terminato con successo \n", getpid());
}

return 0;
}
