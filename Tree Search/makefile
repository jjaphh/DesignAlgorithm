#makefile

CC = gcc
CFLAGS = -Wall -std=c11 -g

all: q1 q2

q1: q1.o
	$(CC) $(CFLAGS) -o q1 q1.o -lm

q1.o: q1.c
	$(CC) $(CFLAGS) -c q1.c -o q1.o -lm

q2: q2.o
	$(CC) $(CFLAGS) -o q2 q2.o -lm

q2.o: q2.c
	$(CC) $(CFLAGS) -c q2.c -o q2.o -lm
clean:
	rm -rf *.o q1 q2