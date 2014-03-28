CFLAGS := -std=c99 -pedantic -Wall -Wextra -g -Wno-unused

include Makefile.inc

.PHONY: clean debug
.SUFFIXES: .o .c

test.o: test.c bxml.h

.c.o:
	gcc $(CFLAGS) -O3 -c -o $@ $<

clean:
	rm -f test *.o *.core

debug:
	gdb test test.core
