#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>


int main (int argc, char** argv) {

if (argc!=2) {
	printf("Usa: ./sleep secondi \n");
	return -1;
}

int pid=fork();

//sono il figlio
if (pid==0) {
	execl("/bin/sleep", "sleep", argv[1], NULL);
}
else {
	waitpid(pid, NULL, 0);
	printf("Processo padre %d. Processo figlio %d. \n", (int)getpid(), (int)getppid());
}


return 0;
}
