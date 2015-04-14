CC=clang
CFLAGS=-g -Wall
LINKCOM=-Iincludes -Llibs
RPATH=-Wl,-rpath,libs
LIBS=-lgraphics -llogger -lconfig -lBBBio -lm
XFLAGS=`pkg-config --cflags --libs x11`

all: test

main: gui src/main.c
	$(CC) $(CFLAGS) $(LINKCOM) $(RPATH) src/main.c gui.o -o main $(LIBS)

test: src/test.c
	$(CC) $(CFLAGS) $(LINKCOM) $(RPATH) src/test.c -o main $(LIBS)

gui: src/gui.c
	$(CC) $(CFALGS) $(LINKCOM) -c src/gui.c 

clean:
	rm main
