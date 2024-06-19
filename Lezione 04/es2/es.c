#include <stdio.h>
#include <stdlib.h>
#define N 10



int main(int argc, char* argv[]) {

float* M= malloc(sizeof(float)*N*N);

//inizializzo le celle
for (int i=0;i<N;i++) {
	for (int j=0;j<N;j++) {
		M[i*N+j]=(i+j)/(2.0);
	}
}
	
FILE* stream;
FILE* stream2;
stream=fopen("mat_dump.dat", "wb"); //file salvato in formato binario
stream2=fopen("mat_dump.txt", "w"); //file salvato in formato float

//salvo matrice nei due file ("mat_dump.dat" e "mat_dump.txt")
for (int i=0;i<N;i++) {
	for (int j=0;j<N;j++) {
		fprintf(stream, "%f ", M[i*N+j]);
		fprintf(stream2, "%f ", M[i*N+j]);
	}
		fprintf(stream, "\n");
		fprintf(stream2, "\n");
}


fclose(stream);
fclose(stream2);

return 0;
}
