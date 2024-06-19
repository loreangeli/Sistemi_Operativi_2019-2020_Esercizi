compilare da terminale:
gcc -std=c99 -Wall tokenizer.h -c -o tokenizer1.o
gcc -std=c99 -Wall tokenizer.c -c -o tokenizer2.o
ar rvs myfirstlib.a tokenizer1.o tokenizer2.o 
gcc main.c -o myprog -L . -lmyf
./myprog miofile.txt miofileo.txt


NOTE: libmyf.a rappresenta una libreria statica. 


Libreria Dinamica: l'eseguibile contiene il main e la libreria non è fusa all'eseguibile e quindi il loader cerca la libreria dinamica e le fonde insieme al momento del caricamento in memoria.
