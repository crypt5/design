#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <string.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_label.h"

struct label_data_t{
  int text_color;
  int border_color;
  int border_width;
  int background;
};

void paint_label(GUI* g, WIDGET* w)
{
  struct label_data_t* data=NULL;
  data=w->widget_data;
  if(w->height!=0){
    XSetForeground(g->dsp,g->draw,g->bgColor);
    XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
  }
  w->width=XTextWidth(g->font,w->string,strlen(w->string));
  w->height=g->font->ascent*2;
  XSetForeground(g->dsp,g->draw,g->bgColor);
  XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
  //TODO Text Color
  //TODO Border and color
  //TODO Enabled/disabled
  XDrawString(g->dsp,g->mainWindow,g->text,w->x,w->y+w->height/2,(char*)w->string,strlen((char*)w->string));
}

WIDGET* create_label(char* message,int x, int y)
{
  WIDGET* w=NULL;
  struct label_data_t* data=NULL;
  char* s=NULL;

  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Widget Malloc Failed\n");
    exit(-1);
  }
  data=malloc(sizeof(struct label_data_t));
  if(data==NULL){
    printf("Label data malloc failed\n");
    exit(-1);
  }
  data->text_color=-1;
  data->border_color=-1;
  data->border_width=-1;
  data->background=-1;

  s=malloc(strlen(message)+1);
  if(s==NULL){
    printf("String malloc failed\n");
    exit(-1);
  }
  strcpy(s,message);

  w->type=LABEL;
  w->flags=NONE;
  w->enable=1;
  w->x=x;
  w->y=y;
  w->width=0;
  w->height=0;
  w->call=NULL;
  w->paint=paint_label;
  w->click=NULL;
  w->select=NULL;
  w->ufree=destroy_label;
  w->string=s;
  w->data=NULL;
  w->widget_data=data;

  return w;
}

void destroy_label(WIDGET* w)
{
  if(w==NULL){
    printf("Label is NULL!\n");
    exit(-2);
  }
  free(w->string);
  free(w->widget_data);
}

void set_label_background(WIDGET* l,int ARGB)
{
  struct label_data_t* data=NULL;
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  data=l->widget_data;
  data->background=ARGB;
}

void set_label_text_color(WIDGET* l,int ARGB)
{
  struct label_data_t* data=NULL;
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  data=l->widget_data;
  data->text_color=ARGB;
}

void set_label_text(WIDGET* l,char* message)
{
  char* s=NULL;
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  free(l->string);

  s=malloc(strlen(message)+1);
  if(s==NULL){
    printf("String malloc failed\n");
    exit(-2);
  }
  strcpy(s,message);
  l->string=s;
}

void set_label_border(WIDGET* l, int ARGB, int thickness)
{
  struct label_data_t* data=NULL;
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  data=l->widget_data;
  data->border_color=ARGB;
  data->border_width=thickness;
}

void set_label_enable(WIDGET* l, int enable)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(enable==1||enable==0)
    l->enable=enable;
  else
    printf("Invalid Enable\n");
}

int get_label_background(WIDGET* l)
{
  struct label_data_t* data=NULL;
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  data=l->widget_data;
  return data->background;
}

int get_label_text_color(WIDGET* l)
{
  struct label_data_t* data=NULL;
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  data=l->widget_data;
  return data->text_color;
}

char* get_label_text(WIDGET* l)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  return l->string;
}

void get_label_border(WIDGET* l,int* re_ARGB, int* re_thickness)
{
  struct label_data_t* data=NULL;
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  data=l->widget_data;
  *re_ARGB=data->border_color;
  *re_thickness=data->border_width;
}

int get_label_enable(WIDGET* l)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  return l->enable;
}

void set_label_click_callback(WIDGET* l,void(*ucallback)(WIDGET* self,void* data),void* data)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  l->call=ucallback;
  l->data=data;
}
