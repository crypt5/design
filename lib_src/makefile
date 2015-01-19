CC=clang
CFLAGS=-g -Wall
OFLAGS=-c -Iheaders -fPIC
GUI_LIBS=graphics_widget.o

build-libs: logger config gui

#data structure builds (.o files)
queue: code/queue.c headers/queue.h
	$(CC) $(CFLAGS) $(OFLAGS) code/queue.c

link-list: code/link.c headers/link.h
	$(CC) $(CFLAGS) $(OFLAGS) code/link.c

# logger build steps 
logger: queue code/logger.c   
	$(CC) $(CFLAGS) $(OFLAGS) code/logger.c
	$(CC) -shared -o output/liblogger.so logger.o queue.o
	cp headers/logger.h output/logger.h

# config build steps 
config: link-list code/config.c 
	$(CC) $(CFLAGS) $(OFLAGS) code/config.c
	$(CC) -shared -o output/libconfig.so config.o link.o
	cp headers/config.h output/config.h

# GUI build steps
gui: $(GUI_LIBS) link-list code/graphics.c  
	$(CC) $(CFLAGS) $(OFLAGS) code/graphics.c
	$(CC) -shared -o output/libgraphics.so $(GUI_LIBS) link.o graphics.o
	cp headers/graphics*.h output/


# GUI Elements Build
graphics_widget.o: code/graphics_widget.c
	$(CC) $(CFLAGS) $(OFLAGS) code/graphics_widget.c