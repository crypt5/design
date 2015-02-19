#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_radio_button.h"

void paint_radio(GUI* g, WIDGET* w)
{
  w->height=g->font->ascent*2;
  w->width=XTextWidth(g->font,w->string,strlen(w->string))+30;
  XSetForeground(g->dsp,g->draw,g->whiteColor);
  XFillArc(g->dsp,g->mainWindow,g->draw,w->x+3,w->y+(g->font->ascent/6),w->height-5,w->height-5,0,360*64);
  XSetForeground(g->dsp,g->draw,g->blackColor);
  XDrawArc(g->dsp,g->mainWindow,g->draw,w->x+3,w->y+(g->font->ascent/6),w->height-5,w->height-5,0,360*64);
  XDrawString(g->dsp,g->mainWindow,g->text,w->x+28,w->y+w->height-(g->font->ascent/2),(char*)w->string,strlen((char*)w->string));

  if(*(int*)w->widget_data==1){
    XSetForeground(g->dsp,g->draw,0x000000AA);
    XFillArc(g->dsp,g->mainWindow,g->draw,w->x+4+(w->height-5)/4,w->y+(g->font->ascent/2),10,10,0,360*64);
  }
}

void paint_radio_click(GUI* g, WIDGET* w)
{
  if(*(int*)w->widget_data==0)
    *(int*)w->widget_data=1;
  else
    *(int*)w->widget_data=0;

  if(*(int*)w->widget_data==1){
    XSetForeground(g->dsp,g->draw,0x000000AA);
    XFillArc(g->dsp,g->mainWindow,g->draw,w->x+4+(w->height-5)/4,w->y+(g->font->ascent/2),10,10,0,360*64);
  }
}

WIDGET* create_radio_button(char* text,int x,int y)
{
  WIDGET* w=NULL;
  char* s=NULL;
  int* d=NULL;
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

  d=malloc(sizeof(int));
  if(d==NULL){
    printf("Data malloc failed!\n");
    exit(-1);
  }
  *d=0;

  w->type=RADIO_BUTTON;
  w->flags=CLICKABLE;
  w->enable=1;
  w->x=x;
  w->y=y;
  w->height=0;
  w->width=0;
  w->call=NULL;
  w->paint=paint_radio;
  w->click=paint_radio_click;
  w->select=NULL;
  w->key_press=NULL;
  w->ufree=destroy_radio_button;
  w->string=s;
  w->data=NULL;
  w->widget_data=d;

  return w;
}

void destroy_radio_button(WIDGET* w)
{
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  free(w->string);
  free(w->widget_data);
}

void set_radio_button_callback(WIDGET* w,void(*ucallback)(GUI* g,WIDGET* self, void* data),void* data)
{
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  w->call=ucallback;
  w->data=data;
}

void remove_radio_button_callback(WIDGET* w)
{
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  w->call=NULL;
  w->data=NULL;
}

void set_radio_button_check(WIDGET* w,int check)
{
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  if(check==1)
    *(int*)w->widget_data=1;
  else if (check==0)
    *(int*)w->widget_data=0;
  else
    printf("Invalid Value, Checkbox not changed\n");
}

int get_radio_button_check(WIDGET* w)
{
  if(w==NULL){
    printf("Radio Button is NULL!\n");
    exit(-1);
  }
  if(w->type!=RADIO_BUTTON){
    printf("Not a Radio Button!\n");
    exit(-2);
  }
  return *(int*)w->widget_data;
}
