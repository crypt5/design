#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <X11/Xlib.h>
#include "link.h"

typedef struct graphics_t GUI;
struct graphics_t{
  Display* dsp;
  Window mainWindow;
  int blackColor;
  int whiteColor;
  int bgColor;
  pthread_mutex_t lock;
  pthread_t tid;
  int run;
  Atom wm_protocols;
  Atom wm_delete_window;
  LIST* widgets;
  GC text;
  GC draw;
  XFontStruct* font;
};

#include "graphics_widget.h"

//These functions need to be called in this order
GUI* init_gui();
void set_main_background(GUI* g,int RGB); // Optional 
void set_main_icon(GUI* g,char* filename);//Optional //TODO
void create_main_window(GUI* g,char* title);
void set_main_size(GUI* g,int height, int width);

void show_main(GUI* g);

int gui_running(GUI* g);

void add_to_main(GUI* g,WIDGET* w);

void shutdown_gui(GUI* g);
void destroy_gui(GUI* g);
#endif
