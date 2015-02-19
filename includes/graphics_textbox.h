#ifndef GRAPHICS_TEXTBOX_H_
#define GRAPHICS_TEXTBOX_H_

#include "graphics.h"
#include "graphics_widget.h"

WIDGET* create_textbox(int x,int y,int max_length);
void destroy_textbox(WIDGET* w);

void set_textbox_text(WIDGET* w,char* text);//TODO
void set_textbox_text_color(WIDGET* w,int ARGB);//TODO
void set_textbox_background_color(WIDGET* w,int ARGB);//TODO
void set_textbox_enable(WIDGET* w, int enable);//TODO

char* get_textbox_text(WIDGET* w,char* text);//TODO
int get_textbox_text_color(WIDGET* w,int ARGB);//TODO
int get_textbox_background_color(WIDGET* w,int ARGB);//TODO
int get_textbox_enable(WIDGET* w, int enable);//TODO

#endif
