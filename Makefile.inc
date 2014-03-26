test: test.o bxml.o
	gcc -o $@ test.o bxml.o

bxml.o: bxml.c bxml.h
