CC =  gcc
CFLAGS = -Wall
.PHONY: test1 clean

myprog: libmyf.so src/myprog.c 
	gcc src/myprog.c -o bin/myprog -Wl,-rpath,./lib -L./lib -lmyf

libmyf.so: binarytree.o include/binarytree.h include/utils.h
	gcc -shared -o lib/libmyf.so lib/binarytree.o
		  
binarytree.o: src/binarytree.c
	gcc -std=c99 -Wall src/binarytree.c -c -fPIC -o lib/binarytree.o

test1:
	gcc -std=c99 -Wall src/binarytree.c -c -fPIC -o lib/binarytree.o
	gcc -shared -o lib/libmyf.so lib/binarytree.o
	gcc src/myprog.c -o bin/myprog -Wl,-rpath,./lib -L./lib -lmyf
	./bin/myprog 5 -2 28 > testfile/out.txt
	diff testfile/out.txt testfile/test1-out_atteso.txt -s

clean: 
	rm -f lib/binarytree.o
	rm -f lib/libmyf.so
	rm -f bin/myprog

