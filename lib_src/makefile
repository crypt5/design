CC=clang
CFLAGS=-g -Wall
OFLAGS=-c -Iheaders -fPIC
GUI_LIBS=graphics_label.o graphics_button.o graphics_radio_button.o graphics_checkbox.o graphics_border.o graphics_titled_border.o graphics_textbox.o

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

#GUI Elements builds
graphics_label.o: code/graphics_label.c
	$(CC) $(CFALGS) $(OFLAGS) code/graphics_label.c

graphics_button.o: code/graphics_button.c
	$(CC) $(CFALGS) $(OFLAGS) code/graphics_button.c

graphics_radio_button.o: code/graphics_radio_button.c
	$(CC) $(CFALGS) $(OFLAGS) code/graphics_radio_button.c

graphics_checkbox.o: code/graphics_checkbox.c
	$(CC) $(CFALGS) $(OFLAGS) code/graphics_checkbox.c

graphics_border.o: code/graphics_border.c
	$(CC) $(CFALGS) $(OFLAGS) code/graphics_border.c

graphics_titled_border.o: code/graphics_titled_border.c
	$(CC) $(CFALGS) $(OFLAGS) code/graphics_titled_border.c

graphics_textbox.o: code/graphics_textbox.c
	$(CC) $(CFALGS) $(OFLAGS) code/graphics_textbox.c