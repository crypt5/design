#ifndef GRAPHICS_H_
#define GRAPHICS_H_

typedef struct graphics_t GUI;

#include "graphics_widget.h"

//These functions need to be called in this order
extern GUI* init_gui();
extern void set_main_background(GUI* g,int RGB); // Optional 
extern void set_main_icon(GUI* g,char* filename);//Optional //TODO
extern void create_main_window(GUI* g,char* title);
extern void set_main_size(GUI* g,int height, int width);
extern void show_main(GUI* g);

extern int gui_running(GUI* g);

extern void add_to_main(GUI* g,WIDGET* w);

extern void destroy_gui(GUI* g);
#endif
