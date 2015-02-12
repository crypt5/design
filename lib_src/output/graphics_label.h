#ifndef GRAPHICS_LABEL_H_
#define GRAPHICS_LABEL_H_

#include "graphics_widget.h"

WIDGET* create_label(char* message,int x, int y);

void set_label_background(WIDGET* l,int ARGB);
void set_label_text_color(WIDGET* l,int ARGB);
void set_label_text(WIDGET* l,char* message);
void set_label_onclick(WIDGET* l,void(*ucallback)(void* data));

int get_label_background(WIDGET* l);
int get_label_text_color(WIDGET* l);
char* get_label_text(WIDGET* l);

void destroy_label(void* l);

#endif
