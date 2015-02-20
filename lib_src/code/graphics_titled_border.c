#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_titled_border.h"

void paint_titled_border(GUI* g, WIDGET* w)
{
  int width;
  XSetForeground(g->dsp,g->draw,g->blackColor);
  width=XTextWidth(g->font,w->string,strlen(w->string))+2;
  XSetLineAttributes(g->dsp,g->draw,*(int*)w->widget_data,LineSolid,CapButt,JoinMiter);
  XDrawRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
  XSetLineAttributes(g->dsp,g->draw,0,LineSolid,CapButt,JoinMiter);
  XSetForeground(g->dsp,g->draw,g->bgColor);
  XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x+5,w->y-(*(int*)w->widget_data/2),width,*(int*)w->widget_data);
  XDrawString(g->dsp,g->mainWindow,g->text,w->x+6,w->y+(g->font->ascent/2),(char*)w->string,strlen((char*)w->string));
}

WIDGET* create_titled_border(char* text,int x,int y,int height,int width)
{
  WIDGET* w=NULL;
  char* s=NULL;
  int* d=NULL;
  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Widget is NULL!\n");
    exit(-1);
  }
  s=malloc(strlen(text)+1);
  if(s==NULL){
    printf("String malloc failed!\n");
    exit(-1);
  }
  d=malloc(sizeof(int));
  if(d==NULL){
    printf("Data malloc failed\n");
    exit(-1);
  }
  strcpy(s,text);
  *d=1;

  w->type=TITLE_BORDER;
  w->flags=NONE;
  w->enable=1;
  w->visible=1;
  w->x=x;
  w->y=y;
  w->height=height;
  w->width=width;
  w->call=NULL;
  w->paint=paint_titled_border;
  w->click=NULL;
  w->select=NULL;
  w->key_press=NULL;
  w->ufree=destroy_titled_border;
  w->string=s;
  w->data=NULL;
  w->widget_data=d;

  return w;
}

void destroy_titled_border(WIDGET* w)
{
  if(w==NULL){
    printf("Titled Border is NULL\n");
    exit(-2);
  }
  if(w->type!=TITLE_BORDER){
    printf("Nort a Titled Border!\n");
    exit(-2);
  }
  free(w->string);
  free(w->widget_data);
}
