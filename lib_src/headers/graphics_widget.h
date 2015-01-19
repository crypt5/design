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

extern WIDGET* create_label(char* message,int x,int y);

extern void destroy_widget(void* w);
extern int comp_widget(void* w1,void* w2);

#endif
