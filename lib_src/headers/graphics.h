#ifndef GRAPHICS_H_
#define GRAPHICS_H_

// Graphics Lib includes
#include "graphics_label.h"

typedef struct graphics_t GUI;

GUI* init_gui();
void destroy_gui(GUI* g);

#endif
