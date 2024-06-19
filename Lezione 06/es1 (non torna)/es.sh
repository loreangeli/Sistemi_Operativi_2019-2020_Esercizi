#!/bin/bash                    

#Le condizioni di confronto tra numeri si fanno con:
#-eq   equal
#-ne   not equal
#-gt   greater than
#-ge  greater than or equal to
#-lt   less than
#-le  less than or equal to

if [ $# -ne 1 ]; then                        # se il numero di argomenti e' diverso da 1
    echo usa: $(basename $0) nomedirectory   # rimuove da $0 il prefisso più lungo che termina con '/'
    exit -1
fi
dir=$1
if [ ! -d $dir ]; then                       # controlla l'esistenza della directory dir
    echo "L'argomento $dir non e' una directory"
    exit 1;   
fi

bdir=$(basename $dir)

if [ -w $bdir.tar ]; then                     # il file esiste ed è scrivibile
    echo -n "il file $bdir.tar.gz esiste gia', sovrascriverlo (S/N)?"
    read yn                                 # il comando read permette di leggere in input (bisogna inserire da tastiera S/N)
    if [ x"$yn" != x"S" ]; then             # confronta la stringa yn e la strings "S" #il problema e' se do invio senza digitare nulla.
														  #Se metto la variabile tra virgolette la cosa funzione perche' avro "" != "S"
														  #mentre se non metto le virgolette avrei un errore di sintassi perche' l'if sarebbe senza una 														  #parte della condizione    [   != "S" ]
          exit 0;
    fi
    rm -f $bdir.tar.gz
fi

echo "creo l'archivio con nome $bdir.tar.gz"

tar cf $bdir.tar $dir 2 >& error.txt        # appende l’output sullo std-error nel file error.txt (da error a output)
if [ $? -ne 0 ]; then                         # controlla che il comando sia andato a buon fine. La variabile "?" contiene l'exit status dell'ultimo comando eseguito. Se è 0 vuol dire che l'uscita è avvenuta senza errori.
    echo "Errore nella creazione dell'archivio"
    exit 1
fi

gzip $bdir.tar 2 >& error.txt                  # appende l’output sullo std-error nel file error.txt
if [ !$? -ne 0 ]; then                         # controlla che il comando sia andato a buon fine. La variabile "?" contiene l'exit status dell'ultimo comando eseguito. Se è 0 vuol dire che l'uscita è avvenuta senza errori.
    echo
    echo "Errore nella compressione dell'archivio"
    exit 1
fi

echo "archivio creato con successo, il contenuto dell’archivio e':"
tar tzvf $bdir.tar.gz   2 >& 1           # redirige lo std-error sullo std-output
exit 0


