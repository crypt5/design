#ifndef GRAPHICS_TEXTBOX_H_
#define GRAPHICS_TEXTBOX_H_

#include "graphics.h"
#include "graphics_widget.h"

WIDGET* create_textbox(int x,int y,int max_length);
void destroy_textbox(WIDGET* w);

void set_textbox_text(WIDGET* w,char* text);
void set_textbox_text_color(WIDGET* w,int ARGB);
void set_textbox_background_color(WIDGET* w,int ARGB);
void set_textbox_enable(WIDGET* w, int enable);
void set_textbox_visible(WIDGET* w,int visible);
void set_textbox_editable(WIDGET* w, int edit);

char* get_textbox_text(WIDGET* w);
int get_textbox_text_color(WIDGET* w);
int get_textbox_background_color(WIDGET* w);
int get_textbox_enable(WIDGET* w);
int get_textbox_visible(WIDGET* w);
int get_textbox_editable(WIDGET* w);

#endif
