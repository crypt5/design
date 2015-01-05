CC=gcc
CFLAGS=-g -Wall
OFLAGS=-c -Iheaders -fPIC

build-libs: logger

test: logger test.c
	$(CC) $(CFLAGS) -Wl,-rpath,output -Loutput test.c -o test -llogger -lpthread

#data type builds (.o files)
queue: code/queue.c headers/queue.h
	$(CC) $(CFLAGS) $(OFLAGS) code/queue.c

# logger build steps
logger: queue logger.o log-share log-copy
	rm logger.o queue.o

logger.o: code/logger.c headers/logger.h 
	$(CC) $(CFLAGS) $(OFLAGS) code/logger.c

log-share: logger.o
	$(CC) -shared -o output/liblogger.so logger.o queue.o

log-copy:
	cp headers/logger.h output/logger.h
