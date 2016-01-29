CC=clang
CFLAGS=-g -Wall -std=gnu11
LINKCOM=-Iincludes 
RPATH=-Wl,-rpath,libs
LIBS=-Llibs -lgraphics -lpthread -llogger -lconfig -lBBBio -ldata_logger -lm
OBJS=gui.o callbacks.o control.o

all: main calibrate

main: gui control src/main.c
	$(CC) $(CFLAGS) $(LINKCOM) $(RPATH) src/main.c $(OBJS) -o main $(LIBS)

gui: callbacks src/gui.c
	$(CC) $(CFLAGS) $(LINKCOM) -c src/gui.c

callbacks: src/callbacks.c
	$(CC) $(CFLAGS) $(LINKCOM) -c src/callbacks.c

control: src/control.c
	$(CC) $(CFLAGS) $(LINKCOM) -c src/control.c
	
calibrate: src/calibrate.c
	$(CC) $(CFLAGS) $(LINKCOM) $(RPATH) src/calibrate.c $(OBJS) -o calibrate $(LIBS)
	
clean:
	rm main *.o
