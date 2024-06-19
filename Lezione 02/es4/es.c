#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* 
Scrivere un programma C che prende in input 3 argomenti, i primi due argomenti sono obbligatori, il terzo e' opzionale. Il primo argomento è il nome di un file di input in formato testuale contenente stringhe di parole.
Il secondo argomento corrisponde ad un file testuale di output che dovra' contenere tutte le parole del primo file, scritte al contrario una per ogni riga.

Se il file di output esiste gia', questo dovra' essere sovrascritto se il terzo argomento del programma o non è stato specificato oppure se tale argomento è diverso dal carattere 'a'. Se il terzo argomento è uguale al carattere 'a' l'output del programma dovrà essere appeso in fondo al file. 


char *fgets(char *s, int size, FILE *stream);
IMPORTANTE: La fgets inserisce alla fine della sequenza di caratteri il carattere newline e il carattere di terminazione stringa '\0'. 
La funzione fgets legge una sequenza di caratteri dal file indicato, e li pone nell'area puntata da s, fino a quando non incontra un carattere newline, fino a quando non ha letto n-1 caratteri, o fino a quando non incontra EOF. 
*/


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


int main(int argc, char *argv[]) {


char* arg1= argv[1];
char x[1024];
FILE* stream;
//fopen apre il file con due parametri: il nome del file e il modo con cui si
//vuole aprire. "r" read, "w" write, "a" append aggiunge dati alla fine del file
stream=fopen (arg1, "r");
/* controlla se il file miofile.txt viene aperto */

if (stream == NULL)
{
printf("Non posso aprire il file %s \n", "miofile.txt");
exit(1);
}



//scriviamo sul file di output
FILE* stream2;
char* arg2= argv[2];
char* tmp;
//controllo la presenza di argv[3]
if (argc==4 && strcmp(argv[3],"a")==0) {
	stream2=fopen(arg2, "a");
}
else stream2=fopen(arg2, "w");

/* controlla se il file miofileo.txt viene aperto */
if (stream2 == NULL)
{
printf("Non posso aprire il file %s \n", "miofileo.txt");
exit(1);
}

//strtok
char* save_ptr=NULL;
char* src=NULL;
char* first=NULL;
int dim=0;


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


fclose(stream);
fclose(stream2);


    return 0;
}
