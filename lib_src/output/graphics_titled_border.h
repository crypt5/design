#ifndef GRAPHICS_TITLED_BORDER_H_
#define GRAPHICS_TITLED_BORDER_H_

#include "graphics_widget.h"
#include "graphics.h"

WIDGET* create_titled_border(char* title,int x,int y,int height,int width);
void destroy_titled_border(WIDGET* w);

void set_titled_border_text(WIDGET *w,char* text);//TODO
void set_titled_border_color(WIDGET* w,int ARGB);//TODO
void set_titled_border_text_color(WIDGET* w,int ARGB);//TODO

char* get_titled_border_text(WIDGET* w);//TODO
int get_titled_border_color(WIDGET* w);//TODO
int get_titled_border_text_color(WIDGET* w);//TODO

#endif
