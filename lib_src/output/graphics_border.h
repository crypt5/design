#ifndef GRAPHICS_BORDER_H_
#define GRAPHICS_BORDER_H_

#include "graphics.h"
#include "graphics_widget.h"

WIDGET* create_simple_border(int x,int y,int height,int width);
void destroy_simple_border(WIDGET* w);

void set_simple_border_color(WIDGET* w, int ARGB);//TODO
void set_simple_border_thickness(WIDGET* w,int thickness);//TODO

int get_simple_border_color(WIDGET* w);//TODO
int get_simple_border_thickness(WIDGET* w);//TODO

#endif
