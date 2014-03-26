CFLAGS := -std=c99 -pedantic -Wall -Wextra -g

include Makefile.inc

.PHONY: clean debug
.SUFFIXES: .o .c

.c.o:
	gcc $(CFLAGS) -O3 -c -o $@ $<

clean:
	rm -f test *.o *.core

debug:
	gdb test test.core
