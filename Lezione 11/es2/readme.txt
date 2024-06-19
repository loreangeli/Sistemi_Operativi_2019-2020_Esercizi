compila con:
gcc -g -Wall -pthread -o server server.c

PS: l'ultima versione è ovviamente quella con più bug corretti
In questa implementazione il client prende le parole come argomenti da terminale (es: ./client "ciao" "ehi") e termina. Non rimane quindi in attesa ma fa le proprie operazioni e poi termina.
