#ifndef GRAPHICS_H_
#define GRAPHICS_H_

typedef struct graphics_t GUI;

// Graphics Lib includes
#include "graphics_widget.h"

//These functions need to be called in this order
GUI* init_gui();
void set_main_background(GUI* g,int RGB);
void set_main_icon(GUI* g,char* filename);//TODO
void create_main_window(GUI* g,char* title);
void set_main_size(GUI* g,int height, int width);
void show_main(GUI* g);
void destroy_gui(GUI* g);

#endif
