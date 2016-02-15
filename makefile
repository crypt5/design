CC=clang
CFLAGS=-g -Wall
LIBS=-Llibs -lgraphics -lpthread -llogger -lconfig -lBBBio -ldata_logger -lm
OBJS=gui.o callbacks.o control.o

all: main calibrate

main: gui control src/main.c
	$(CC) $(CFLAGS) src/main.c $(OBJS) -o main $(LIBS)

gui: callbacks src/gui.c
	$(CC) $(CFLAGS) -c src/gui.c

callbacks: src/callbacks.c
	$(CC) $(CFLAGS) -c src/callbacks.c

control: src/control.c
	$(CC) $(CFLAGS) -c src/control.c

calibrate: src/calibrate.c
	$(CC) $(CFLAGS) src/calibrate.c $(OBJS) -o calibrate $(LIBS)

clean:
	rm *.o
