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

static inline void closechannel(int channel[2]) {
	close(channel[0]);
	close(channel[1]);
}

int main(int argc, char** argv) {
	if (argc<2) {
		printf("Usa: ./pipedec [numero] \n");
		return 1;
	}
	if (atoi(argv[1])<1) {
		printf("Inserisci valore positivo \n");
		return 1;
	}

	//pipe 1
	int channel[2];
	if (pipe(channel)<0) {
		perror("pipe");
		return errno;
	}	
	//pipe 2
	int channel2[2];
	if (pipe(channel2)<0) {
		perror("pipe");
		return errno;
	}

	pid_t pid1, pid2;
	//creo figlio 1
	pid1=fork();
	if (pid1==-1) {
		perror("forking primo figlio");		
		return errno;
	}

	if (pid1==0) {
		if (dup2(channel[0],0)<0) { //faccio si che stdin non legga da stdin ma dalla pipe1
			perror("dup2");
			closechannel(channel);
			return errno;
			} 
		if (dup2(channel2[1],1)<0) { //faccio si che stdout non scriva sulla stdout ma sulla pipe2
			perror("dup2");
			closechannel(channel2);
			return errno;
		}
		closechannel(channel);
		closechannel(channel2);

		execlp("/home/lorenzo/dec", "dec", argv[1], NULL);
		perror("execln");
	}

	//creo figlio 2
	pid2=fork();
	if (pid2==-1) {
		perror("forking primo figlio");		
		return errno;
	}

	if (pid2==0) {
		if (dup2(channel2[0],0)<0) { //faccio si che stdin non legga da stdin ma dalla pipe2.
			perror("dup2");
			closechannel(channel2);
			return errno;
		} 
		if (dup2(channel[1],1)<0) { //faccio si che stdout non scriva sulla stdout ma sulla pipe1. (ogni volta che il programma dec vuole scrivere sullo stdout viene indirizzata la scrittura sulla pipe)
			perror("dup2");
			closechannel(channel);
			return errno;
		}
		closechannel(channel);
		closechannel(channel2);
		execlp("/home/lorenzo/dec", "dec", NULL);	
		perror("execln");
	}

	closechannel(channel);
	closechannel(channel2);

	//attendo terminazione dei figli
	if (waitpid(pid1, NULL, 0)<0) { perror("waitpid pid1"); return -1;}
	if (waitpid(pid2, NULL, 0)<0) { perror("waitpid pid2"); return -1;}

return 0;
}
