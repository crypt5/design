#ifndef GRAPHICS_H_
#define GRAPHICS_H_

typedef struct graphics_t GUI;
#define MODE_FULLSCREEN 1
#define MODE_WINDOW 2

// Graphics Lib includes
#include "graphics_widget.h"

GUI* init_gui();
void create_main_window(GUI* g,char* title, int mode);
void set_main_size(GUI* g,int height, int width);
void show_main(GUI* g);
void destroy_gui(GUI* g);

#endif
