#define _GNU_SOURCE //ci va messo altrimento errore "implicit declaration of function ‘strtok_r’; did you mean ‘strtok’?"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char* reverse(char* stringa) {
	size_t i, j;
	int d= strlen(stringa);
	if (stringa[d-1]=='\n') {
		char* str= malloc(sizeof(char)*5);
		for (i=0;i<d-1;i++) {
			char temp= stringa[i];
			str[d-i-2]=temp;
		}		
		return str;
	}
	else {
		for (i=0, j=strlen(stringa)-1; i<strlen(stringa)/2;i++, j--) {
			char temp=stringa[i];
			stringa[i]=stringa[j];
			stringa[j]=temp;
		}
	}

	return stringa;
}


//stream -> file da cui prendo le stringhe
//stream2 -> file in cui copio le stringhe
void tokenizer (FILE* stream, FILE* stream2) {
char* src=NULL;
char* first=NULL;
char* save_ptr=NULL;
char* tmp=NULL;
int dim=0;
char x[1024];

		while (!feof(stream)) {
				  {
				      if ( (src=fgets(x, 1024, stream)) != NULL)
				      {
							first = strtok_r(src, " ", &save_ptr);	
							
							while(first!=NULL){
								tmp=reverse(first);
								dim=strlen(tmp)-1;
								if ( tmp[dim]=='\n') 
									fprintf(stream2, "%s", tmp);	
								else 	fprintf(stream2, "%s \n", tmp);
								first = strtok_r(NULL, " ", &save_ptr);												
							}
				      }
				      else
				      {
				          printf("Trovato EOF (End of File) \n");
				          break;
				      }
				  }
		}

}

