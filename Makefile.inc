test: test.o bxml.o
	$(CC) -o $@ test.o bxml.o

bxml.o: bxml.c bxml.h
