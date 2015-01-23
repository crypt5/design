#ifndef GRAPHICS_WIDGET_H_
#define GRAPHICS_WIDGET_H_

//Widget types
#define LABEL 1
#define BUTTON 2

//Widget FLags
#define CLICKABLE 1

typedef void(*callback)(void* data);
typedef struct graphics_widget_t WIDGET;
struct graphics_widget_t{
  char type;
  int x;
  int y;
  int height;
  int width;
  char flags;
  callback call;
  char* string;
  void* data;
};


extern WIDGET* create_label(char* message,int x,int y);

extern WIDGET* create_button(char* message,int x, int y,void(*ucallback)(void*),void* data);

extern void destroy_widget(void* w);

#endif
