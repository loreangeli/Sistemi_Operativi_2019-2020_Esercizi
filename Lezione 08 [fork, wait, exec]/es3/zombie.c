#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>


int main (int argc, char** argv) {
int pid=1;
int status;
int N=atoi ( argv[1] );
int i=0;

	while (pid!=0 && i<N) {
			pid=fork();
			i++;
	}

if (pid) { //sono il padre
	sleep(5);
	for (int j=0;j<N;j++) {
		pid=waitpid(-1, &status,0);
		if (WIFEXITED(status)) //risulta vera (diversa da zero) se il child e' uscito normalmente
			printf("Terminato processo figlio con pid: %d \n", pid); 
	}
}
else {
	printf("Processo figlio con pid: %d \n", getpid());
	exit(0);
}


return 0;
}
