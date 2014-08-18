test: test.o bxml.o libbxml.a
	$(CC) -o $@ test.o -L. -lbxml

libbxml.a: bxml.o
	ar rcs $@ bxml.o

bxml.o: bxml.c bxml.h
