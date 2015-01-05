CC=gcc
CFLAGS=-g -Wall
LINKCOM=-Iincludes -Llibs
RPATH=-Wl,-rpath,libs
LIBS=-llogger

all: main

main: src/main.c
	$(CC) $(CFLAGS) $(LINKCOM) $(RPATH) src/main.c -o main $(LIBS)

clean:
	rm main