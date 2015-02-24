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

  if(w->visible!=1){
    if(w->height!=0){
      XSetForeground(g->dsp,g->draw,g->bgColor);
      XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
    }
    w->width=XTextWidth(g->font,w->string,strlen(w->string))+6;
    w->height=g->font->ascent*2;
    return;
  }

  data=w->widget_data;
  if(w->height!=0){
    XSetForeground(g->dsp,g->draw,g->bgColor);
    XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
  }
  w->width=XTextWidth(g->font,w->string,strlen(w->string))+6;
  w->height=g->font->ascent*2;
  if(data->background>=0)
    XSetForeground(g->dsp,g->draw,w->enable==1 ? data->background : to_gray(data->background));
  else
    XSetForeground(g->dsp,g->draw,g->bgColor);
  XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
  if(data->text_color>0){
    XSetForeground(g->dsp,g->text,w->enable==1 ? data->text_color : to_gray(data->text_color));
    XDrawString(g->dsp,g->mainWindow,g->text,w->x+3,w->y+w->height/2+(w->height/4),(char*)w->string,strlen((char*)w->string));
    XSetForeground(g->dsp,g->text,g->blackColor);
  }
  else{
    XDrawString(g->dsp,g->mainWindow,g->text,w->x+3,w->y+w->height/2+(w->height/4),(char*)w->string,strlen((char*)w->string));
  }
  if(data->border_color>=0){
    XSetForeground(g->dsp,g->draw,w->enable==1 ? data->border_color : to_gray(data->border_color));
    XSetLineAttributes(g->dsp,g->draw,data->border_width,LineSolid,CapButt,JoinMiter);
    XDrawRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
    XSetLineAttributes(g->dsp,g->draw,0,LineSolid,CapButt,JoinMiter);
  }
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
  w->visible=1;
  w->x=x;
  w->y=y;
  w->width=0;
  w->height=0;
  w->call=NULL;
  w->paint=paint_label;
  w->click=NULL;
  w->select=NULL;
  w->key_press=NULL;
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
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
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
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
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
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
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
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
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
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  if(enable==1){
    l->enable=enable;
    if(l->call!=NULL)
      l->flags=l->flags|CLICKABLE;
    if(l->select!=NULL)
      l->flags=l->flags|SELECTABLE;
  }
  else if(enable==0){
    l->enable=enable;
    l->flags=NONE;
  }
  else{
    printf("Invalid Enable Flag\nNo Action Taken\n");
  }
}

void set_label_visible(WIDGET* l, int visible)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  if(visible==1){
    l->visible=visible;
  }
  else if(visible==0){
    l->visible=visible;
  }
  else{
    printf("Invalid visible Flag\nNo Action Taken\n");
  }
}

int get_label_background(WIDGET* l)
{
  struct label_data_t* data=NULL;
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
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
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
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
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
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
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
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
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  return l->enable;
}

int get_label_visible(WIDGET* l)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  return l->visible;
}

void set_label_click_callback(WIDGET* l,void(*ucallback)(GUI* g,WIDGET* self,void* data),void* data)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  l->call=ucallback;
  l->data=data;
  l->flags=l->flags|CLICKABLE;
}

void set_label_paint_click(WIDGET* l,void(*uclick)(GUI* g, WIDGET* l))
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  l->click=uclick;
  l->flags=l->flags|CLICKABLE;
}

void set_label_paint_select(WIDGET* l,void(*uselect)(GUI* g, WIDGET* l),void(*ukey_press)(GUI* g,WIDGET* w,char key))
{  
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  l->select=uselect;
  l->key_press=ukey_press;
  l->flags=l->flags|SELECTABLE;
}

void remove_label_click_callback(WIDGET* l)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  l->call=NULL;
  l->data=NULL;
  l->flags=l->flags&(0xFF^CLICKABLE);
}

void remove_label_paint_click(WIDGET* l)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  l->click=NULL;
  l->flags=l->flags&(0xFF^CLICKABLE);
}

void remove_label_paint_select(WIDGET* l)
{
  if(l==NULL){
    printf("Label is NULL!!!\n");
    exit(-1);
  }
  if(l->type!=LABEL){
    printf("NOT A LABEL!!!\n");
    exit(-3);
  }
  l->select=NULL;
  l->key_press=NULL;
  l->flags=l->flags&(0xFF^SELECTABLE);
}
