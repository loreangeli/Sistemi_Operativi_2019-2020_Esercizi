//prende come argomento una directory e stampa tutto il contenuto (file, directory) presenti in quella directory
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>



//ritorna 1 se è un file, 0 se è una directory
int is_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}


void print_dir(char dir[]) {

	if (is_file(dir)==1) {
		printf("Non è una directory");
		return;
	}

	printf("%s \n", dir);

	char buffer[100];
   struct stat path_stat;
	struct dirent* file;
	struct passwd* pwd;

	struct group* gid;

	DIR* d;

	//apertura directory
	if (( d=opendir(dir) ) == NULL ) {
		perror("opening dir");
		exit(EXIT_FAILURE);
	}

	//stampa contenuto presente in quella directory
	while ( (errno=0, file=readdir(d)) != NULL ) { //readdir restituisce sia file che directory

		getcwd(buffer, 100);
		strcat(buffer,"/");
		strcat(buffer, file->d_name);
		stat(buffer, &path_stat);

		printf("%s  ", file->d_name); //nome file
		printf("%ld  ", (long)path_stat.st_ino); //i-nodo
		printf("%ld  ", (long)path_stat.st_nlink); //hard-link
		printf("%ld  ", (long)path_stat.st_uid); //ID owner
		//tipo di file
		printf( (S_ISDIR(path_stat.st_mode)) ? "d" : "-");
		printf( (S_ISLNK(path_stat.st_mode)) ? "l" : "-");
		printf( (S_ISREG(path_stat.st_mode)) ? "f" : "-");
		printf( (S_ISCHR(path_stat.st_mode)) ? "spc" : "-");
		printf( (S_ISBLK(path_stat.st_mode)) ? "spb" : "-");			
		//bit di protezione
		printf("  ");
	   printf( (path_stat.st_mode & S_IRUSR) ? "r" : "-");
	   printf( (path_stat.st_mode & S_IWUSR) ? "w" : "-");
		printf( (path_stat.st_mode & S_IXUSR) ? "x" : "-");
	   printf( (path_stat.st_mode & S_IRGRP) ? "r" : "-");
		printf( (path_stat.st_mode & S_IWGRP) ? "w" : "-");
		printf( (path_stat.st_mode & S_IXGRP) ? "x" : "-");
		printf( (path_stat.st_mode & S_IROTH) ? "r" : "-");
		printf( (path_stat.st_mode & S_IWOTH) ? "w" : "-");
		printf( (path_stat.st_mode & S_IXOTH) ? "x" : "-");
		//uid
		printf(" ");
		pwd=getpwuid(path_stat.st_uid);
		printf("%d", pwd->pw_uid);
		printf(" ");
		//gid
		gid=getgrgid(path_stat.st_gid);
		printf("%d", gid->gr_gid);	
		printf(" ");
		//size
		printf("%ld  ", (long)path_stat.st_size); //size file
		printf(" ");
		//time
		time_t tmp;
		tmp=(path_stat.st_mtime); //tmp di tipo time_t
		gmtime(&tmp);
		printf("%s", ctime(&tmp));

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

if (argc<2) {
	printf("Usa: ./printdir nomedirectory1 .. nomedirectoryn");
	return -1;
}

for (int i=1;i<argc;i++) {
	print_dir(argv[i]);
	if (i!=(argc-1))
	printf("\n");
}

return 0;
}
