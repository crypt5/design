CC=clang
CFLAGS=-g -Wall
LINKCOM=-Iincludes -Llibs
RPATH=-Wl,-rpath,libs
LIBS=-lpthread -lgraphics 
XFLAGS=`pkg-config --cflags --libs x11`

all: main

main: libs src/main.c
	$(CC) $(CFLAGS) $(LINKCOM) $(RPATH) $(XFLAGS) src/main.c -o main $(LIBS)

libs:
	cd lib_src && make
	cp lib_src/output/*.so libs
	cp lib_src/output/*.h includes

clean:
	rm main