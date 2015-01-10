CC=gcc
CFLAGS=-g -Wall
OFLAGS=-c -Iheaders -fPIC
GUI_LIBS=graphics_label.o

build-libs: logger config gui

#data structure builds (.o files)
queue: code/queue.c headers/queue.h
	$(CC) $(CFLAGS) $(OFLAGS) code/queue.c

link-list: code/link.c headers/link.h
	$(CC) $(CFLAGS) $(OFLAGS) code/link.c

# logger build steps 
logger: queue code/logger.c headers/logger.h  
	$(CC) $(CFLAGS) $(OFLAGS) code/logger.c
	$(CC) -shared -o output/liblogger.so logger.o queue.o
	cp headers/logger.h output/logger.h

# config build steps 
config: link-list code/config.c headers/config.h
	$(CC) $(CFLAGS) $(OFLAGS) code/config.c
	$(CC) -shared -o output/libconfig.so config.o link.o
	cp headers/config.h output/config.h

# GUI build steps
gui: $(GUI_LIBS) code/graphics.c headers/graphics.h
	$(CC) $(CFLAGS) $(OFLAGS) code/graphics.c
	$(CC) -shared -o output/libgraphics.so graphics.o $(GUI_LIBS)
	cp headers/graphics*.h output/


# GUI Elements Build
graphics_label.o: code/graphics_label.c headers/graphics_label.h
	$(CC) $(CFLAGS) $(OFLAGS) code/graphics_label.c