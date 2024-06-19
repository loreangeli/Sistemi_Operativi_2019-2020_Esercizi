#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[]) {
char* punt=NULL;
FILE* stream;
FILE* stream2;

stream=fopen("/etc/passwd", "r"); 
stream2=fopen("output.txt", "w"); 
char* x= malloc(sizeof(char)*1024);

fgets(x, 1024, stream);

while (!feof(stream)) {
punt=strtok(x, ":");
fprintf(stream2,"%s \n", punt);
fgets(x, 1024, stream);
}



fclose(stream);
fclose(stream2);

return 0;
}
