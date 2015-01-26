#ifndef GRAPHICS_WIDGET_H_
#define GRAPHICS_WIDGET_H_

//Widget Types
#define LABEL 1
#define BUTTON 2
#define RADIO_BUTTON 3
#define CHECKBOX 4
#define BORDER 5
#define TITLE_BORDER 6

//Widget Action FLags
#define NONE 0
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
extern WIDGET* create_radio_button(char* message, int x, int y);
extern WIDGET* create_checkbox(char* message, int x, int y);
extern WIDGET* create_border(int x, int y, int height, int width,int thickness);
extern WIDGET* create_titled_border(char* message, int x, int y, int height, int width,int thickness);

extern void destroy_widget(void* w);

#endif
