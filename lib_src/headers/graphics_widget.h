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
typedef void(*callback)(void* data);
typedef void(*painter)(GUI* g, WIDGET* w);
typedef void(*on_click)(GUI* g, WIDGET* w);
typedef void(*on_select)(GUI* g, WIDGET* w);
typedef void(*free_widget)(void* widget);
struct graphics_widget_t{
  char type;
  int x;
  int y;
  int height;
  int width;
  char flags;
  callback call;     //Button Press callback
  painter paint;     //Function to paint widget
  on_click click;    //Function to paint when widget is clicked
  on_select select;  //Function to print widget when selected
  free_widget ufree; //Function to free widget's memory
  char* string;
  void* data;        //callback data
  void* widget_data; //Special widget data
};

struct textbox_data_t{
  int max_length;
  int current_pos;
};

WIDGET* create_label(char* message,int x,int y);
WIDGET* create_button(char* message,int x, int y,void(*ucallback)(void*),void* data);
WIDGET* create_radio_button(char* message, int x, int y);
WIDGET* create_checkbox(char* message, int x, int y);
WIDGET* create_border(int x, int y, int height, int width,int thickness);
WIDGET* create_titled_border(char* message, int x, int y, int height, int width,int thickness);
WIDGET* create_textbox(int x, int y, int max_length);

void destroy_widget(void* w);

//Widget Interaction Functions
char* get_text(WIDGET* w);
int set_text(WIDGET* w,char* message);
int is_checked(WIDGET* w);

#endif
