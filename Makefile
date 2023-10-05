CC = gcc
WFLAGS = -Wall -lm
EXE = program
all: hmac

hmac: HMAC.c
	$(CC) HMAC.c $(WFLAGS)

clean:
	rm -f *.out