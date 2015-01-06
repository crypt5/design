CC=clang
CFLAGS=-g -Wall
OFLAGS=-c -Iheaders -fPIC

build-libs: logger

# Test file build
test: config test.c
	$(CC) $(CFLAGS) -Wl,-rpath,output -Loutput test.c -o test -lconfig

#data structure builds (.o files)
queue: code/queue.c headers/queue.h
	$(CC) $(CFLAGS) $(OFLAGS) code/queue.c

link-list: code/link.c headers/link.h
	$(CC) $(CFLAGS) $(OFLAGS) code/link.c

# logger build steps (next 4)
logger: queue logger.o log-share log-copy
	rm logger.o queue.o

logger.o: code/logger.c headers/logger.h 
	$(CC) $(CFLAGS) $(OFLAGS) code/logger.c

log-share:
	$(CC) -shared -o output/liblogger.so logger.o queue.o

log-copy:
	cp headers/logger.h output/logger.h

# config build steps (next 4)
config: link-list config.o config-share config-copy
	rm config.o link.o

config.o: code/config.c headers/config.h
	$(CC) $(CFLAGS) $(OFLAGS) code/config.c

config-share:
	$(CC) -shared -o output/libconfig.so config.o link.o

config-copy:
	cp headers/config.h output/config.h