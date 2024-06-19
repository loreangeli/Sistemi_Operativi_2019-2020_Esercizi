#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tokenizer.h"



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

tokenizer(stream, stream2);

fclose(stream);
fclose(stream2);

return 0;
}
