CFLAGS=-std=c99 -pedantic -Wall -Wextra -g

test: test.o xml.o
	gcc -o $@ test.o xml.o

.PHONY: clean debug
.SUFFIXES: .o .c

xml.o: xml.c xml.h

.c.o:
	gcc $(CFLAGS) -O3 -c -o $@ $<

clean:
	rm -f test *.o *.core

debug:
	gdb test test.core
