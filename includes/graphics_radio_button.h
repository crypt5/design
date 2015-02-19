#ifndef GRAPHICS_RADIO_BUTTON_H_
#define GRAPHICS_RADIO_BUTTON_H_

#include "graphics_widget.h"
#include "graphics.h"

WIDGET* create_radio_button(char* text,int x,int y);
void destroy_radio_button(WIDGET* w);

void set_radio_button_text(WIDGET* w,char* text);//TODO
void set_radio_button_text_color(WIDGET* w,int color);//TODO
void set_radio_button_check(WIDGET* w,int check);
void set_radio_button_enable(WIDGET* w, int enable);//TODO

char* get_radio_button_text(WIDGET* w);//TODO
int get_radio_button_text_color(WIDGET* w);//TODO
int get_radio_button_check(WIDGET* w);
int get_radio_button_enable(WIDGET* w);//TODO

void set_radio_button_paint_select(WIDGET* w,void(*uselect)(GUI* g, WIDGET* l),void(*ukey_press)(WIDGET* w, char key));//TODO
void set_radio_button_callback(WIDGET* w,void(*ucallback)(GUI* g,WIDGET* self, void* data),void* data);

void remove_radio_button_paint_select(WIDGET* w);//TODO
void remove_radio_button_callback(WIDGET* w);

#endif
