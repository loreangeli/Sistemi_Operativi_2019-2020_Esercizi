#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv) {
int lettir; //ci salvo il numero di caratteri letti

char buffer[200];
if (argc<2)
	printf("Usa: ./mycat file1 [filen]");

int fd; //file descriptor
/* apro file in sola lettura */
if(( fd=open(argv[1], O_RDONLY)) == -1) {
	perror("apertura file");
	exit(EXIT_FAILURE);
}

/* leggo */
for (int i=0;i<argc-1;i++) {
	lettir=1;
	/* apro file in sola lettura */
	if(( fd=open(argv[i+1], O_RDONLY)) == -1) {
		perror("apertura file");
		exit(EXIT_FAILURE);
	}
	while((lettir=read(fd,buffer,100))>0) {
		
		if (write(1, buffer, lettir)==-1) {
			perror("write");
			exit(EXIT_FAILURE);
		}
	}
}


if(close(fd)==-1) { 
	perror("Errore chiusura");
	exit(EXIT_FAILURE);
}


return 0;
}
