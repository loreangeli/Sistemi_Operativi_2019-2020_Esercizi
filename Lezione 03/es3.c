#include <stdlib.h>
#include <stdio.h>

#define dimN 16
#define dimM  8
#define CHECK_PTR(M,str) if (M==NULL) { perror("str") ; exit(0) ;}
#define ELEM(M,i,j) M[dimM*i+j]
#define PRINTMAT(M,dimN,dimM) for(int i=0;i<dimN;i++) { for(int j=0;j<dimM;j++){printf("%ld ", M[i]);  } printf("\n");}


int main() {
    long *M = malloc(dimN*dimM*sizeof(long));
    CHECK_PTR(M, "malloc"); 

    for(size_t i=0;i<dimN;++i)
		for(size_t j=0;j<dimM;++j)		
	    ELEM(M,i,j) = i+j;    
    
    PRINTMAT(M, dimN, dimM);

    return 0;
}
