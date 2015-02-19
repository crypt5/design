#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_button.h"

void paint_button(GUI* g, WIDGET* w)
{
  if(w->width!=0){
    XSetForeground(g->dsp,g->draw,g->bgColor);
    XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
  }
  w->width=XTextWidth(g->font,w->string,strlen(w->string))+20;
  w->height=g->font->ascent;
  XSetForeground(g->dsp,g->draw,0x00AAAAAA);
  XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height*2);
  //TODO Text Color
  //TODO Enable/Disable
  XDrawString(g->dsp,g->mainWindow,g->text,w->x+10,w->y+(w->height+w->height/2),w->string,strlen(w->string));
  w->height=w->height*2;
}

void paint_click(GUI* g, WIDGET* w)
{
  XSetForeground(g->dsp,g->draw,0);
  XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
}

WIDGET* create_button(char* text, int x, int y)
{
  WIDGET* w=NULL;
  char* d=NULL;
  int* color=NULL;
  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Button Malloc failed!\n");
    exit(-1);
  }
  d=malloc(strlen(text)+1);
  if(d==NULL){
    printf("String malloc failed\n");
    exit(-1);
  }
  color=malloc(sizeof(int));
  if(color==NULL){
    printf("Text color malloc failed\n");
    exit(-1);
  }
  *color=-1;
  strcpy(d,text);
  w->type=BUTTON;
  w->flags=CLICKABLE;
  w->enable=1;
  w->x=x;
  w->y=y;
  w->height=0;
  w->width=0;
  w->call=NULL;
  w->paint=paint_button;
  w->click=paint_click;
  w->select=NULL;
  w->key_press=NULL;
  w->ufree=destroy_button;
  w->string=d;
  w->data=NULL;
  w->widget_data=color;

  return w;
}

void destroy_button(WIDGET* w)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  free(w->widget_data);
  free(w->string);
}

void set_button_callback(WIDGET* w,void(*ucallback)(GUI* g,WIDGET* self, void* data),void* data)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  w->call=ucallback;
  w->data=data;

}

void set_button_paint_select(WIDGET* w,void(*uselect)(GUI* g, WIDGET* w),void(*ukey_press)(GUI* g,WIDGET* w,char key))
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  w->select=uselect;
  w->type=w->type|SELECTABLE;
  w->key_press=ukey_press;
}

void remove_button_click_callback(WIDGET* w)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  w->call=NULL;
  w->data=NULL;
}

void remove_button_paint_select(WIDGET* w)
{
 if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  w->select=NULL;
  w->key_press=NULL;
  w->flags=w->flags&(0xFF^SELECTABLE);
}
