#ifndef GRAPHICS_H_
#define GRAPHICS_H_

typedef struct graphics_t GUI;

// Graphics Lib includes
#include "graphics_label.h"

GUI* init_gui();
void create_main_window(GUI* g);
void destroy_gui(GUI* g);

#endif
