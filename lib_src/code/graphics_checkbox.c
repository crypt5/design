#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_radio_button.h"

void paint_checkbox(GUI* g, WIDGET* w)
{
  w->height=g->font->ascent*2;
  w->width=XTextWidth(g->font,w->string,strlen(w->string))+30;
  XSetForeground(g->dsp,g->draw,g->whiteColor);
  XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x+5,w->y+(w->height/5),15,15);
  XSetForeground(g->dsp,g->draw,g->blackColor);
  XDrawRectangle(g->dsp,g->mainWindow,g->draw,w->x+5,w->y+(w->height/5),15,15);
  XDrawString(g->dsp,g->mainWindow,g->text,w->x+28,w->y+w->height-(g->font->ascent/2),(char*)w->string,strlen((char*)w->string));

  if(*(int*)w->widget_data==1){
    XSetForeground(g->dsp,g->draw,0x0000AA00);
    //Short Leg
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+9,w->x+12,w->y+14);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+10,w->x+12,w->y+15);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+11,w->x+12,w->y+16);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+12,w->x+12,w->y+17);
    //Long Leg
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+12,w->y+14,w->x+17,w->y+5);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+12,w->y+15,w->x+17,w->y+6);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+12,w->y+16,w->x+17,w->y+7);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+12,w->y+17,w->x+17,w->y+8);
  }
}

void paint_checkbox_click(GUI* g, WIDGET* w)
{
  if(*(int*)w->widget_data==0)
    *(int*)w->widget_data=1;
  else
    *(int*)w->widget_data=0;

  if(*(int*)w->widget_data==1){
    XSetForeground(g->dsp,g->draw,0x0000AA00);
    //Short Leg
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+9,w->x+12,w->y+14);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+10,w->x+12,w->y+15);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+11,w->x+12,w->y+16);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+12,w->x+12,w->y+17);
    //Long Leg
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+12,w->y+14,w->x+17,w->y+5);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+12,w->y+15,w->x+17,w->y+6);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+12,w->y+16,w->x+17,w->y+7);
    XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+12,w->y+17,w->x+17,w->y+8);
  }
}

WIDGET* create_checkbox(char* text,int x,int y)
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
}
