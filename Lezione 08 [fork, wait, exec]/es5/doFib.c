#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* 
 * Calcola ricorsivamente il numero di Fibonacci dell'argomento 'n'.
 * La soluzione deve forkare un nuovo processo che esegue una sola 
 * chiamata di 'doFib'.
 * Se doPrint e' 1 allora la funzione lo stampa, altrimenti il
 * numero calcolato va passato al processo padre.
 */
/*

  if (i == 0) return 0;
  else if (i == 1) return 1;
  else return fibonacci(i-1) + fibonacci(i-2);*/

static void doFib(int n, int doPrint) {
	int sum=0;
	int status1, status2=0;

	if(n==0)
		exit(0);
	if (n==1)
		exit(1);
	
	int pid, pid2=-1;
	int ris, ris2=0;

	if ( (pid=fork())==-1 ) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid!=0) {
		if ( (pid2=fork())==-1 ) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
	}

	if (pid==0) {
		doFib(n-1, 0);
	}
	if (pid2==0) {
		doFib(n-2, 0);
	}

	if (pid!=0) {
		waitpid(pid, &status1, 0);
		ris=WEXITSTATUS(status1);
		/*printf("ris: %d \n", ris);*/
	}
	
	if (pid2!=0) {
		waitpid(pid2, &status2, 0);
		ris2=WEXITSTATUS(status2);
		/*printf("ris2: %d \n", ris2);*/
	}

	sum=sum+ris+ris2;
	if (doPrint==0)
		exit(sum);
	if (doPrint==1) {
		printf("%d \n", sum);
	}	
}


int main(int argc, char *argv[]) {
    // questo programma puo' calcolare i numeri di Fibonacci solo fino a 13.  
    const int NMAX=13;
    int arg;
    
    if(argc != 2){
	fprintf(stderr, "Usage: %s <num>\n", argv[0]);
	return EXIT_FAILURE;
    }
    arg = atoi(argv[1]);
    if(arg < 0 || arg > NMAX){
	fprintf(stderr, "num deve essere compreso tra 0 e 13\n");
	return EXIT_FAILURE;
    }   
    doFib(arg, 1);
    return 0;
}
