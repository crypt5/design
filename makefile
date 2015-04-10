CC=clang
CFLAGS=-g -Wall
LINKCOM=-Iincludes -Llibs
RPATH=-Wl,-rpath,libs
LIBS=-lgraphics -llogger -lconfig -lBBBio -lm
XFLAGS=`pkg-config --cflags --libs x11`

all: test

test: src/test.c
	$(CC) $(CFLAGS) $(LINKCOM) $(RPATH) src/test.c -o main $(LIBS)

clean:
	rm main
