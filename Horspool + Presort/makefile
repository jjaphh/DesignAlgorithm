#makefile

CC = gcc
CFLAGS = -Wall -std=c11 -g

all: p1 p2

p1: p1.o
	$(CC) $(CFLAGS) -o p1 p1.o -lm

p1.o: p1.c
	$(CC) $(CFLAGS) -c p1.c -o p1.o -lm

p2: p2.o
	$(CC) $(CFLAGS) -o p2 p2.o -lm

p2.o: p2.c
	$(CC) $(CFLAGS) -c p2.c -o p2.o -lm

clean:
	rm -rf *.o p1 p2