CC=gcc
CFLAGS=-g -Wall


build-libs: logger


logger: logger.o log-share log-copy
	rm logger.o

logger.o: code/logger.c headers/logger.h 
	$(CC) $(CFLAGS) -Iheaders -fPIC -c code/logger.c

log-share: logger.o
	$(CC) -shared -o output/liblogger.so logger.o

log-copy:
	cp headers/logger.h output/logger.h
