CC 	?= cc
CFLAGS	:= -std=c99 -pedantic -Wall -Wextra -g -Wno-unused-parameter

include Makefile.inc

.PHONY: clean debug
.SUFFIXES: .o .c

test.o: test.c bxml.h

.c.o:
	$(CC) $(CFLAGS) -O3 -c -o $@ $<

clean:
	rm -f test *.o *.core

debug:
	gdb test test.core
