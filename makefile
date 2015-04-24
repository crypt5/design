CC=clang
CFLAGS=-g -Wall
LINKCOM=-Iincludes -Llibs
RPATH=-Wl,-rpath,libs
LIBS=-lgraphics -llogger -lconfig -lBBBio -ldata_logger
OBJS=gui.o callbacks.o control.o

all: main

main: gui control src/main.c
	$(CC) $(CFLAGS) $(LINKCOM) $(RPATH) src/main.c $(OBJS) -o main $(LIBS)

gui: callbacks src/gui.c
	$(CC) $(CFLAGS) $(LINKCOM) -c src/gui.c

callbacks: src/callbacks.c
	$(CC) $(CFLAGS) $(LINKCOM) -c src/callbacks.c

control: src/control.c
	$(CC) $(CFLAGS) $(LINKCOM) -c src/control.c
clean:
	rm main *.o
