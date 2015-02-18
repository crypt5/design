#ifndef GRAPHICS_BUTTON_H_
#define GRAPHICS_BUTTON_H_

#include "graphics_widget.h"

WIDGET* create_button(char* text, int x, int y);
void destroy_button(WIDGET* w);//TODO

void set_button_text_color(WIDGET* w,int ARGB);//TODO
void set_button_text(WIDGET* w, char* message);//TODO
void set_button_enable(WIDGET* w,int enable);//TODO

int get_button_text_color(WIDGET* w);//TODO
char* get_button_text(WIDGET* w);//TODO
int get_button_enable(WIDGET* w);//TODO


void set_button_callback(WIDGET* w,void(*ucallback)(WIDGET* self, void* data),void* data);//TODO
void set_button_paint_select(WIDGET* w,void(*uselect)(GUI* g, WIDGET* w),void(*ukey_press)(WIDGET* w,char key));//TODO

void remove_button_click_callback(WIDGET* l);//TODO
void remove_button_paint_select(WIDGET* l);//TODO

#endif