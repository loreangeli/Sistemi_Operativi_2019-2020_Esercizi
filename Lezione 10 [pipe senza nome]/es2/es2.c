#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

static inline void closechannel(int channel[2]) {
	close(channel[0]);
	close(channel[1]);
}

int main (int argc, char** argv) {
char* arg1=malloc(sizeof(char)*20);

while (1) {
	printf("Operazione: ");
	scanf("%s", arg1);
	printf("Risultato: ");
	fflush(stdout); //è fondamentale perchè mi svuota il buffer dello stdout scrivendomi sul terminale "Risultato: ".
	int channel[2];
	if (pipe(channel)<0) {
		perror("pipe");
		return errno;
	}
	pid_t pid1, pid2;
	switch(pid1=fork()) {
		case 0: { //primo figlio, eseguirà echo che scriverebbe nello stdout
			if (dup2(channel[1],1)<0) { //faccio si che stdout non scriva sulla stdout ma sulla pipe
				perror("dup2");
				closechannel(channel);
				return errno;
			}
			
			closechannel(channel);
				
			execlp("/bin/echo", "echo", arg1, NULL);
			perror("execln");
		}
		case -1: {
			perror("forking primo figlio");		
			return errno;
		}
		default:; //sono il padre
	}

	
	switch(pid2=fork()) {
		case 0: { //secondo figlio eseguirà bc
			if (dup2(channel[0],0)<0) { //da ora in poi chiunque legga dallo stdin legge da channel[0]
				perror("dup2");
				closechannel(channel);
				return errno;
			}
			closechannel(channel);
			execlp("/usr/bin/bc", "bc", NULL);	
			perror("execlp");		
			return errno;
		}
		case -1: {
			perror("forking secondo figlio");	
			return errno;
		}
		default:; //padre
	}
	closechannel(channel);
	
	//attendo terminazione dei figli
	if (waitpid(pid1, NULL, 0)<0) { perror("waitpid pid1"); return -1;}
	if (waitpid(pid2, NULL, 0)<0) { perror("waitpid pid2"); return -1;}	

} //chiudo while

return 0;
}
