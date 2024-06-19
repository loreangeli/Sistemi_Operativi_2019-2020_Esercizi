//prende come argomento una directory e stampa tutto il contenuto (file, directory) presenti in quella directory
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

//ritorna 1 se è un file, 0 se è una directory
int is_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}


void print_dir(char dir[]) {
	printf("info: 1->file 0->directory \n");
	printf("Stampo contenuto directory: %s \n", dir);

	if (is_file(dir)==1) {
		printf("Non è una directory");
		return;
	}


	struct dirent* file;
	DIR* d;

	//apertura directory
	if (( d=opendir(dir) ) == NULL ) {
		perror("opening dir");
		exit(EXIT_FAILURE);
	}

	//stampa contenuto presente in quella directory
	while ( (errno=0, file=readdir(d)) != NULL ) { //readdir restituisce sia file che directory
		printf("%d: ",is_file(file->d_name));
		printf("%s \n", file->d_name);
	}

	if (errno!=0) {
		perror("errno!=0");
	}

	//chiusura directory
	if (( closedir(d) == -1 )) {
		perror("closing dir");
		exit(EXIT_FAILURE);
	}

} 

int main(int argc, char *argv[]) {

if (argc!=2) {
	printf("Usa: ./printdir nomedirectory");
	return -1;
}

print_dir(argv[1]);

return 0;
}
