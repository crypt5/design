#ifndef GRAPHICS_WIDGET_H_
#define GRAPHICS_WIDGET_H_

#define LABEL 1
#define BUTTON 2
#define TEXTBOX 3

typedef struct graphics_widget_t WIDGET;
struct graphics_widget_t{
  int type;
  int x;
  int y;
  int height;
  int width;
  void* data;
};

typedef void(*callback)(void* data);
struct button_data_t{
  char* text;
  callback call;
  void* call_data;
};
  

extern WIDGET* create_label(char* message,int x,int y);

extern WIDGET* create_button(char* message,int x, int y,void(*ucallback)(void*),void* data);

extern void destroy_widget(void* w);

#endif
