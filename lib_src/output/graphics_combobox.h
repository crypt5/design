#ifndef GRAPHICS_COMBOBOX_H_
#define GRAPHICS_COMBOBOX_H_

#include "graphics.h"
#include "graphics_widget.h"

WIDGET* create_combobox(int display_width,int x,int y,char*(*ustring)(void* uitem));
void destroy_combobox(WIDGET* w);


void add_item_to_combobox(WIDGET* w,void* item);
void set_combobox_number_to_display(WIDGET* w,int display);//TODO
void set_combobox_background_color(WIDGET* w,int ARGB);//TODO
void set_combobox_text_color(WIDGET* w,int ARGB);//TODO
void set_combobox_selected_item(WIDGET* w,int select);//TODO
void set_combobox_enable(WIDGET* w,int enable);//TODO
void set_combobox_visible(WIDGET* w,int visible);//TODO

void* remove_item_from_combobox(WIDGET* w,int index);
int get_number_of_items_in_combobox(WIDGET* w);
int get_combobox_items_displayed(WIDGET* w);//TODO
int get_combobox_background_color(WIDGET* w);//TODO
int get_combobox_text_color(WIDGET* w);//TODO
int get_combobox_selected_item(WIDGET* w);//TODO
int get_combobox_enable(WIDGET* w);//TODO
int get_combobox_visible(WIDGET* w);//TODO

//TODO clicked Callback
//TODO Selected Callback

#endif
