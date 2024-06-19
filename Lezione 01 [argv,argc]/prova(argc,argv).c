
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 

int main(int argc, char *argv[]) {

//stampa argc
printf("%d \n", argc);

//stampa argv
for (int i=0; i<argc;i++) {
	printf("%s \n", argv[i]);
}

  return 0;
}
