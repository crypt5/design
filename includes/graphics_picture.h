#ifndef GRAPHICS_PICTURE_H_
#define GRAPHICS_PICTURE_H_

#include "graphics.h"
#include "graphics_widget.h"

WIDGET* create_picture(GUI* g,char* filename,int x,int y);
void destroy_picture(WIDGET* w);

void set_picture_visible(WIDGET* w,int visible);//TODO

int get_picture_visible(WIDGET* w);//TODO

void set_label_click_callback(WIDGET* l,void(*ucallback)(GUI* g,WIDGET* self,void* data),void* data);//TODO
void set_label_paint_click(WIDGET* l,void(*uclick)(GUI* g, WIDGET* l));//TODO
void set_label_paint_select(WIDGET* l,void(*uselect)(GUI* g, WIDGET* l),void(*ukey_press)(GUI* g,WIDGET* w, char key));//TODO

void remove_label_click_callback(WIDGET* l);//TODO
void remove_label_paint_click(WIDGET* l);//TODO
void remove_label_paint_select(WIDGET* l);//TODO

#endif
