CC=gcc
CFLAGS=-g -Wall
LINKCOM=-Iincludes -Llibs
RPATH=-Wl,-rpath,libs
LIBS=

all: main

main: libs src/main.c
	$(CC) $(CFLAGS) $(LINKCOM) $(RPATH) src/main.c -o main $(LIBS)

libs:
	cd lib_src && make
	cp lib_src/output/*.so libs
	cp lib_src/output/*.h includes

clean:
	rm main