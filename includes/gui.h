#ifndef GUI_H_
#define GUI_H_

#include "graphics.h"

GUI* build_gui();
void clean_up_gui(GUI* g);

int is_open(GUI* g);

#endif
