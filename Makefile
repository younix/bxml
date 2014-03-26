CFLAGS=-std=c99 -pedantic -Wall -Wextra -g

test: test.o bxml.o
	gcc -o $@ test.o bxml.o

.PHONY: clean debug
.SUFFIXES: .o .c

bxml.o: bxml.c bxml.h

.c.o:
	gcc $(CFLAGS) -O3 -c -o $@ $<

clean:
	rm -f test *.o *.core

debug:
	gdb test test.core
