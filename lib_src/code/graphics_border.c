#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_border.h"

void paint_border(GUI* g, WIDGET* w)
{
  XSetForeground(g->dsp,g->draw,g->blackColor);
  XSetLineAttributes(g->dsp,g->draw,*(int*)w->widget_data,LineSolid,CapButt,JoinMiter);
  XDrawRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
  XSetLineAttributes(g->dsp,g->draw,0,LineSolid,CapButt,JoinMiter);
}

WIDGET* create_simple_border(int x,int y,int height,int width)
{
  WIDGET* w=NULL;
  int* d=NULL;
  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Border Malloc failed!\n");
    exit(-1);
  }
  d=malloc(sizeof(int));
  if(d==NULL){
    printf("Data Malloc Failed!\n");
    exit(-1);
  }
  *d=1;
  w->type=BORDER;
  w->flags=NONE;
  w->enable=1;
  w->x=x;
  w->y=y;
  w->height=height;
  w->width=width;
  w->call=NULL;
  w->paint=paint_border;
  w->click=NULL;
  w->select=NULL;
  w->key_press=NULL;
  w->ufree=destroy_simple_border;
  w->string=NULL;
  w->data=NULL;
  w->widget_data=d;

  return w;
}

void destroy_simple_border(WIDGET* w)
{
  if(w==NULL){
    printf("Border is NULL!\n");
    exit(-2);
  }
  if(w->type!=BORDER){
    printf("Not a Border!\n");
    exit(-2);
  }
  free(w->widget_data);
}
