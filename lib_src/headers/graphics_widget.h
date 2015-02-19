#ifndef GRAPHICS_WIDGET_H_
#define GRAPHICS_WIDGET_H_

//Widget Types
#define LABEL 1
#define BUTTON 2
#define RADIO_BUTTON 3
#define CHECKBOX 4
#define BORDER 5
#define TITLE_BORDER 6
#define TEXTBOX 7

//Widget Action FLags
#define NONE 0 
#define CLICKABLE 1
#define SELECTABLE 2


typedef struct graphics_widget_t WIDGET;
#include "graphics.h"
typedef void(*callback)(WIDGET* self,void* data);
typedef void(*painter)(GUI* g, WIDGET* w);
typedef void(*on_click)(GUI* g, WIDGET* w);
typedef void(*on_select)(GUI* g, WIDGET* w);
typedef void(*on_keypress)(WIDGET* w, char key);
typedef void(*free_widget)(WIDGET* widget);
struct graphics_widget_t{
  char type;
  char flags;
  char enable;
  int x;
  int y;
  int height;
  int width;
  callback call;     //Button Press callback
  painter paint;     //Function to paint widget
  on_click click;    //Function to paint when widget is clicked
  on_select select;  //Function to print widget when selected
  on_keypress key_press; //Function to pass key press to
  free_widget ufree; //Function to free widget's memory
  char* string;
  void* data;        //callback data
  void* widget_data; //Special widget data
};

#include "graphics_label.h"
#include "graphics_button.h"
#include "graphics_radio_button.h"
#include "graphics_checkbox.h"
#include "graphics_border.h"
#include "graphics_titled_border.h"

#endif
