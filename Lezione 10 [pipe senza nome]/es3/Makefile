pipedec: pipedec.o dec
	gcc pipedec.o -o pipedec

dec: dec.o
	gcc dec.o -o dec

pipedec.o: pipedec.c
	gcc -Wall -pedantic -c pipedec.c

dec.o: dec.c
	gcc -Wall -pedantic -c dec.c
