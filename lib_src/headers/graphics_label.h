#ifndef GRAPHICS_LABEL_H_
#define GRAPHICS_LABEL_H_

#include "graphics_widget.h"

WIDGET* create_label(char* message,int x, int y);
void destroy_label(WIDGET* l);

int set_label_background(WIDGET* l,int ARGB);
int set_label_text_color(WIDGET* l,int ARGB);
int set_label_text(WIDGET* l,char* message);
int set_label_border(WIDGET* l, int ARGB, int thickness);

int get_label_background(WIDGET* l);
int get_label_text_color(WIDGET* l);
char* get_label_text(WIDGET* l);
void get_label_border(WIDGET* l,int* re_ARGB, int* re_thickness);

void set_label_click_callback(WIDGET* l,void(*ucallback)(void* data),void* data);
void set_label_paint_click(WIDGET* l,void(*uclick)(GUI* g, WIDGET* l));
void set_label_paint_select(WIDGET* l,void(*uselect)(GUI* g, WIDGET* l));

void remove_label_click_callback(WIDGET* l);
void remove_label_paint_click(WIDGET* l);
void remove_label_paint_select(WIDGET* l);

#endif
