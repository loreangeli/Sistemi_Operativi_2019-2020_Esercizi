 /*Scrivere una funzione 'mystrcat' con il seguente prototipo:

const char *mystrcat(char* buffer, int buffer_size, char *prima, ...);

La funzione prende un buffer, la lunghezza del buffer in bytes ed almeno uno stringa. Le stringhe possono essere un numero variabile (>1). La funzione concatena tutte le stringhe nel 'buffer' e ritorna il buffer stesso. ATTENZIONE alla gestione della memoria! 
*/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RIALLOCA(buf, newsize) \
    buf= realloc(buf, newsize);
 

char* mystrcat (char *buf, int buffer_sz, char *first, ...) {
	int dim=1; //carattere terminazione

	//calcolo dimensione totale 
	dim=strlen(first)+dim;

   va_list ap;
	va_start (ap, first); //inizializzo la lista

	char* tmp= va_arg(ap, char*);

	while(tmp != NULL) {
		dim=dim+strlen(tmp);
		tmp= va_arg(ap, char*);	
	}

	va_end(ap);

	//concateno stringhe
	RIALLOCA(buf, dim);
	va_list pa;
	va_start (pa, first); //inizializzo la lista
	buf=strcat(buf, first); //la stringa first viene messa in coda alla stringa buf
	char* t= va_arg(pa, char*);

	while(t != NULL) {
		buf=strcat(buf, t);		
		t= va_arg(pa, char*);	
	}

	va_end(pa);	

	return buf;
}  
 

int main(int argc, char *argv[]) {
  if (argc != 7) { printf("troppi pochi argomenti\n"); return -1; }
  char *buffer=NULL;
  RIALLOCA(buffer, 16);  // macro che effettua l'allocazione
  buffer[0]='\0';
  buffer = mystrcat(buffer, 16, argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], NULL);
  printf("%s\n", buffer);     
  free(buffer);
  return 0;
}
