#ifndef GUI_H_
#define GUI_H_

#include "graphics.h" 

GUI* build_gui();
void clean_up_gui(GUI* g);
int is_open(GUI* g);
void my_exit(GUI* g,WIDGET* w, void* d);
#endif
