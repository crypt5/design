#ifndef GRAPHICS_CHECKBOX_H_
#define GRAPHICS_CHECKBOX_H_

#include "graphics_widget.h"
#include "graphics.h"

WIDGET* create_checkbox(char* text,int x,int y);
void destroy_checkbox(WIDGET* w);

void set_checkbox_text(WIDGET* w,char* text);//TODO
void set_checkbox_text_color(WIDGET* w,int color);//TODO
void set_checkbox_check(WIDGET* w,int check);//TODO
void set_checkbox_enable(WIDGET* w, int enable);//TODO

char* get_checkbox_text(WIDGET* w);//TODO
int get_checkbox_text_color(WIDGET* w);//TODO
int get_checkbox_check(WIDGET* w);//TODO
int get_checkbox_enable(WIDGET* w);//TODO

void set_checkbox_paint_select(WIDGET* w,void(*uselect)(GUI* g, WIDGET* l),void(*ukey_press)(WIDGET* w, char key));//TODO
void set_checkbox_callback(WIDGET* w,void(*ucallback)(WIDGET* self, void* data),void* data);//TODO

void remove_checkbox_paint_select(WIDGET* w);//TODO
void remove_checkbox_callback(WIDGET* w);//TODO

#endif
