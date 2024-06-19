#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define MAXARG 10
#define MAXRIGA 15

//funzione utilità che rimuove dalla stringa \n
char* removecapo(char* stringa) {
	int i=0;
	int lun=strlen(stringa);
	int presente=0; //messo a 1 se c'è \n
	//controlla se c'è il carattere \n
	while (i<lun) {
		if (stringa[i]=='\n')
			presente=1;
		i++;
	}
	if (presente==0) return stringa;

	i=0;
	while (stringa[i]!='\n' && i<lun)
		i++;
	if (stringa[i]=='\n')
		stringa[i]='\0';

	return stringa;
}

static void execute(char* arg0, char* argv[]) {
	pid_t pid;
	int status;
	pid=fork();
	
	if (pid==0) {
		execvp(arg0, argv); //pathname e array di argomenti (primo argomento è il nome del programma, ultimo argomento NULL)
	}
	else {
		waitpid(pid, &status, 0);
	}
}


int main(int argc, char** argv) {
	char* riga;
	char path[10];
	size_t n = 1024;
	riga=malloc(n);
	char* token;
	char **arg=malloc(sizeof(char*)*MAXARG);
	int narg;	
	int i=0;

	while(1) {
		printf("> ");
		fflush(stdout);
		getline(&riga, &n, stdin);
		i=0;
		narg=0;

		//tokenizzo riga
		token=strtok(riga, " ");
		while(token!=NULL) {\
			arg[i]=malloc(sizeof(char)*(strlen(token)+1));
			narg++;
			strcpy(arg[i], removecapo(token));
			token=strtok(NULL, " ");
			i++;
		}
	
		arg[i]=NULL;

		if (strcmp(arg[0], "exit")==0)		
			exit(1);
		//path
		strcpy(path, "/bin/");
		strcat(path, removecapo(arg[0]));
		/*printf("path: %s \n", path);*/		
		
		
		execute(path, arg);
	} //chiudo while


return 0;
}
