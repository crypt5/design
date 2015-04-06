CC=clang
CFLAGS=-g -Wall
LINKCOM=-Iincludes -Llibs
RPATH=-Wl,-rpath,libs
LIBS=-lpthread -lgraphics -llogger -lconfig -lXpm -liolib
XFLAGS=`pkg-config --cflags --libs x11`

all: test

test: src/test.c
	$(CC) $(CFLAGS) $(LINKCOM) $(RPATH) $(XFLAGS) src/test.c -o main $(LIBS)

clean:
	rm main
