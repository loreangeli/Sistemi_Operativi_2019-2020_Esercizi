#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

int is_file(char* path) {
	struct stat info;
	if (stat(path, &info)==-1) {
		/* gestione errore */
		printf("error stat file \n");
		return -1; //false
	}
	
	return S_ISREG(info.st_mode);
}

int is_dir(char* path) {
	struct stat info;
	if (stat(path, &info)==-1) {
		/* gestione errore */
		printf("error stat dir \n");
		return -1; //false
	}
	
	return S_ISDIR(info.st_mode);
}

void print_ric (char* dir, char* filetosearch) {

//controlli
if (dir==NULL) {
	printf("directory NULL \n");
	return;
}

if (is_dir(dir)!=1) {
	printf("%s non è una directory", dir);
	return;	
}


DIR* d;
struct stat path_stat;
char* path=NULL;
int lpath;
struct dirent* file;
/* apertura directory dir*/
if((d=opendir(dir))==NULL) {
	perror("opening dir");
	exit(EXIT_FAILURE);
}

	while ((errno=0, file=readdir(d))!=NULL) {
		if ( strcmp(file->d_name, ".")!=0 && strcmp(file->d_name, "..")!=0 ) {
			//assemblo directory
			lpath=sizeof(char)*(2+strlen(file->d_name)+strlen(dir));
			if (path!=NULL)
				free(path);

			path=malloc(lpath);
			strcpy(path, dir);
			strcat(path, "/");
			strcat(path, file->d_name);


			if (is_file(path)==1) {
				if (strcmp(file->d_name,filetosearch)==0) {
					printf("%s ", path);
					//time
					time_t tmp;
					tmp=(path_stat.st_mtime); //tmp di tipo time_t
					gmtime(&tmp);
					printf("%s", ctime(&tmp)); 
				}
			}
			else if (is_dir(path)==1) {
				/*printf("%s è una directory \n", path);*/
				print_ric(path, filetosearch);

			}
		}
	} //whileclose

if (errno!=0) {
	printf("errno!=0 \n");
}

/* libero path */
if (path!=NULL)
	free(path);

/* chiusura directory */
if ((closedir(d)==-1)) {
	perror("closing dir");
	exit(EXIT_FAILURE);
}


}


int main(int argc, char** argv) {
if (argc!=3) {
	printf("Usa: ./myfind directory file");
	return -1;
}

print_ric(argv[1], argv[2]);

return 0;
}
