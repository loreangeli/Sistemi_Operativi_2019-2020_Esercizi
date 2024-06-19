#!/bin/bash
#README: quando compili ricordati di mettere il file output, in questo modo: ./testbash output.txt

OUT=$1 #OUT rappresenta il file output passato allo script

k=0
./client "ciao" "altra"  >&	$OUT
PID[k]=$! #si salva in PID[0] il l'ID del processo eseguito più di recente
((k++))
./client "goodbye" 2>&1 >> $OUT
PID[k]=$! #si salva in PID[0] il l'ID del processo eseguito più di recente
((k++))
./client "ciao" "altra"	2>&1 >> $OUT
PID[k]=$! #si salva in PID[0] il l'ID del processo eseguito più di recente
((k++))
./client "ciao" "altra"	2>&1 >> $OUT
PID[k]=$! #si salva in PID[0] il l'ID del processo eseguito più di recente
((k++))
./client "ciao" "altra"	2>&1 >> $OUT
PID[k]=$! #si salva in PID[0] il l'ID del processo eseguito più di recente
((k++))
./client "ciao" "altra"	2>&1 >> $OUT
PID[k]=$! #si salva in PID[0] il l'ID del processo eseguito più di recente
((k++))
./client "ciao" "altra"	2>&1 >> $OUT
PID[k]=$! #si salva in PID[0] il l'ID del processo eseguito più di recente
((k++))
./client "ciao" "altra"	2>&1 >> $OUT
PID[k]=$! #si salva in PID[0] il l'ID del processo eseguito più di recente
((k++))
./client "ultimo" "messaggio"	2>&1 >> $OUT
PID[k]=$! #si salva in PID[0] il l'ID del processo eseguito più di recente
((k++))

for((i=0;i<k;++i)); do
	wait ${PID} #aspetta il termine del processo PID[0]
done
