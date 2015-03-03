#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_checkbox.h"

struct checkbox_data_t{
  int checked;
  int text_color;
  int check_color;
};

void paint_checkbox(GUI* g, WIDGET* w)
{
  struct checkbox_data_t* data=NULL;
  data=w->widget_data;

  if(w->width!=0){
     XSetForeground(g->dsp,g->draw,g->bgColor);
     XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
  }
  w->height=g->font->ascent*2;
  w->width=XTextWidth(g->font,w->string,strlen(w->string))+20;
  if(w->visible==0){
     XSetForeground(g->dsp,g->draw,g->bgColor);
     XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
    return;
  }
  XSetForeground(g->dsp,g->draw,w->enable==1 ? g->whiteColor : to_gray(g->whiteColor));
  XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y+(w->height/5),15,15);
  XSetForeground(g->dsp,g->draw,g->blackColor);
  XDrawRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y+(w->height/5),15,15);

  if(data->text_color>0){
    XSetForeground(g->dsp,g->text,w->enable==1 ? data->text_color : to_gray(data->text_color));
    XDrawString(g->dsp,g->mainWindow,g->text,w->x+18,w->y+w->height-(g->font->ascent/2),(char*)w->string,strlen((char*)w->string));
    XSetForeground(g->dsp,g->text,g->blackColor);
  }
  else {
    XDrawString(g->dsp,g->mainWindow,g->text,w->x+18,w->y+w->height-(g->font->ascent/2),(char*)w->string,strlen((char*)w->string));
  }

  if(data->checked==1){
    if(data->check_color>0)
      XSetForeground(g->dsp,g->draw,w->enable==1 ? data->check_color : to_gray(data->check_color));
    else
      XSetForeground(g->dsp,g->draw,w->enable==1 ? 0x0000AA00 : to_gray(0x0000AA00));
    //Short Leg
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+2,w->y+9,w->x+7,w->y+14);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+2,w->y+10,w->x+7,w->y+15);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+2,w->y+11,w->x+7,w->y+16);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+2,w->y+12,w->x+7,w->y+17);
    //Long Leg
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+14,w->x+12,w->y+5);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+15,w->x+12,w->y+6);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+16,w->x+12,w->y+7);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+17,w->x+12,w->y+8);
  }
}

void paint_checkbox_click(GUI* g, WIDGET* w)
{
  struct checkbox_data_t* data=NULL;
  data=w->widget_data;
  if(data->checked==0)
    data->checked=1;
  else
    data->checked=0;
}

WIDGET* create_checkbox(char* text,int x,int y)
{
  WIDGET* w=NULL;
  char* s=NULL;
  struct checkbox_data_t* d=NULL;
  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Widget malloc failed!\n");
    exit(-1);
  }
  s=malloc(strlen(text)+1);
  if(s==NULL){
    printf("String malloc failed!\n");
    exit(-2);
  }
  strcpy(s,text);

  d=malloc(sizeof(struct checkbox_data_t));
  if(d==NULL){
    printf("Data malloc failed!\n");
    exit(-1);
  }
  d->checked=0;
  d->text_color=-1;
  d->check_color=-1;

  w->type=CHECKBOX;
  w->flags=CLICKABLE;
  w->enable=1;
  w->visible=1;
  w->x=x;
  w->y=y;
  w->height=0;
  w->width=0;
  w->call=NULL;
  w->paint=paint_checkbox;
  w->click=paint_checkbox_click;
  w->select=NULL;
  w->key_press=NULL;
  w->ufree=destroy_checkbox;
  w->string=s;
  w->data=NULL;
  w->widget_data=d;

  return w;
}

void destroy_checkbox(WIDGET* w)
{
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  free(w->string);
  free(w->widget_data);
  free(w);
}

void set_checkbox_text(WIDGET* w,char* text)
{
  char* s=NULL;
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  free(w->string);
  s=malloc(strlen(text)+1);
  strcpy(s,text);
  w->string=s;
}

void set_checkbox_text_color(WIDGET* w,int color)
{
  struct checkbox_data_t* data=NULL;
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  data=w->widget_data;
  data->text_color=color;
}

void set_checkbox_check_color(WIDGET* w, int color)
{
  struct checkbox_data_t* data=NULL;
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  data=w->widget_data;
  data->check_color=color;
}

void set_checkbox_checked(WIDGET* w,int check)
{
  struct checkbox_data_t* data=NULL;
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  data=w->widget_data;
  if(check==1||check==0)
    data->checked=check;
  else
    printf("Invalid Check data\nNo action taken\n");
}

void set_checkbox_enable(WIDGET* w, int enable)
{
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  if(enable==0||enable==1)
    w->enable=enable;
  else
    printf("Invalid enable flag\nNo action taken\n");
}

void set_checkbox_visible(WIDGET* w, int visible)
{
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  if(visible==0||visible==1)
    w->visible=visible;
  else
    printf("Invalid visible flag\nNo action taken\n");
}

char* get_checkbox_text(WIDGET* w)
{
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  return w->string;
}

int get_checkbox_text_color(WIDGET* w)
{
  struct checkbox_data_t* data=NULL;  
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  data=w->widget_data;
  return data->text_color;
}

int get_checkbox_check_color(WIDGET* w)
{
  struct checkbox_data_t* data=NULL;  
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  data=w->widget_data;
  return data->check_color;
}

int get_checkbox_checked(WIDGET* w)
{
  struct checkbox_data_t* data=NULL;  
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  data=w->widget_data;
  return data->checked;
}

int get_checkbox_enable(WIDGET* w)
{
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  return w->enable;
}

int get_checkbox_visible(WIDGET* w)
{
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  return w->visible;
}

void set_checkbox_paint_select(WIDGET* w,void(*uselect)(GUI* g, WIDGET* l),void(*ukey_press)(GUI* g,WIDGET* w, char key))
{
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  w->flags=w->flags|SELECTABLE;
  w->select=uselect;
  w->key_press=ukey_press;
}

void set_checkbox_callback(WIDGET* w,void(*ucallback)(GUI* g,WIDGET* self, void* data),void* data)
{
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  w->call=ucallback;
  w->data=data;
}

void remove_checkbox_paint_select(WIDGET* w)
{
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  w->flags=w->flags&!SELECTABLE;
  w->select=NULL;
  w->key_press=NULL;
}

void remove_checkbox_callback(WIDGET* w)
{
  if(w==NULL){
    printf("Checkbox is NULL!\n");
    exit(-1);
  }
  if(w->type!=CHECKBOX){
    printf("Not a Checkbox!\n");
    exit(-2);
  }
  w->call=NULL;
  w->data=NULL;
}
