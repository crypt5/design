CC=gcc
CFLAGS=-g -Wall

all: test

test: src/test.c includes/test.h
	$(CC) $(CFALGS) src/test.c -o main

clean:

