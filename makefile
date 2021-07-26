CC=gcc
CFLAGS=-I . -Wall --pedantic -lncurses

all: filman

filman: main.c
	$(CC) $(CFLAGS) main.c -o filman

clean:
	rm filman
