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


void print_ric(char* dir) {
if (dir==NULL)
	return;

//controllo se ho passato una directory
if (is_dir(dir)==0) {
	//printf("%s Non è una directory \n", dir);
	return;
}

DIR* d;
DIR* d2;
struct dirent* file;
int lpath;
char* path=NULL;
char* pathtmp=NULL;
//open directory
if ((d=opendir(dir))==NULL) {
	perror("opening cwd \n");
	exit(EXIT_FAILURE);
}


//scorriamo la directory
printf("Directory: <%s> \n", dir);

		file=readdir(d);

		//creo directory
		lpath=sizeof(char)*(2+strlen(file->d_name)+strlen(dir));
		path=malloc(lpath); //lunghezza path 
		strcpy(path, dir);
		strcat(path,"/");
		strcat(path,file->d_name);
		pathtmp=malloc(lpath);
		strcpy(pathtmp, path);		
	
		printf("path: %s \n", path);
		while (file!=NULL) {//trovato file
				if (is_file(path)==1 && strcmp(".",file->d_name)!=0 && strcmp("..",file->d_name)!=0) {

				struct stat st;

				if (stat(path, &st)==-1) {
					/* gestione errore */
					printf("stat error \n");
			}

				printf("%s ", file->d_name); //nome file
				printf("%ld  ", (long)st.st_size); //size file
				//bit di protezione
				printf( (st.st_mode & S_IRUSR) ? "r" : "-");
				printf( (st.st_mode & S_IWUSR) ? "w" : "-");
				printf( (st.st_mode & S_IXUSR) ? "x" : "-");
				printf( (st.st_mode & S_IRGRP) ? "r" : "-");
				printf( (st.st_mode & S_IWGRP) ? "w" : "-");
				printf( (st.st_mode & S_IXGRP) ? "x" : "-");
				printf( (st.st_mode & S_IROTH) ? "r" : "-");
				printf( (st.st_mode & S_IWOTH) ? "w" : "-");
				printf( (st.st_mode & S_IXOTH) ? "x" : "-");
				printf("\n"); 
			}

			file=readdir(d);

			//creo directory
			if (file!=NULL) {
				lpath=sizeof(char)*(2+strlen(file->d_name)+strlen(dir));
 				free(path);
				path=malloc(lpath);
				strcpy(path, dir);
				strcat(path,"/");
				strcat(path,file->d_name);
			}
			
		}
		

		//open directory d2
		if ((d2=opendir(dir))==NULL) {
			perror("opening cwd \n");
			exit(EXIT_FAILURE);
		}

		//ripristinato path
		free(path);
		path=malloc(sizeof(char)*(2+strlen(pathtmp)));
		strcpy(path, pathtmp); 
		file=readdir(d2);		

		while(file!=NULL ) {
				

				//creo directory
				lpath=sizeof(char)*(2+strlen(file->d_name)+strlen(dir));
				free(path);
				path=malloc(lpath);
				strcpy(path, dir);
				strcat(path,"/");
				strcat(path,file->d_name);

			if (is_dir(path)==1 && strcmp(".",file->d_name)!=0 && strcmp("..",file->d_name)!=0) {
				/*printf("Ho trovato una directory: %s \n", path);*/
				print_ric(path);
			}
				file=readdir(d2);
		}


	if (path!=NULL) {
		free(path);	
	}
	if (pathtmp!=NULL) {
		free(pathtmp);	
	}

	//chiudi directory
	if ((closedir(d)==-1)) {
		exit(EXIT_FAILURE);
	}

	//chiudi directory
	if ((closedir(d2)==-1)) {
		exit(EXIT_FAILURE);
	}

} //chiudi funzione


int main(int argc, char** argv) {
print_ric(argv[1]);

return 0;
}
