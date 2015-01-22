#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "graphics_widget.h"

WIDGET* create_label(char* message,int x,int y)
{
  WIDGET* w=NULL;
  char* d=NULL;

  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Widget Malloc Failed!\n");
    exit(-1);
  }

  d=malloc(strlen(message)+1);
  if(d==NULL){
    printf("Message Malloc Failed!\n");
    exit(-1);
  }
  strcpy(d,message);
  w->type=LABEL;
  w->x=x;
  w->y=y+10;
  w->string=d;

  return w;
}

WIDGET* create_button(char* message,int x, int y,void(*ucallback)(void*),void* data)
{
  WIDGET* w=NULL;
  char* d=NULL;

  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Widget Malloc Failed!\n");
    exit(-1);
  }

  d=malloc(strlen(message)+1);
  if(d==NULL){
    printf("Message Malloc Failed!\n");
    exit(-1);
  }
  strcpy(d,message);
  w->type=BUTTON;
  w->x=x;
  w->y=y+10;
  w->data=data;
  w->call=ucallback;
  w->string=d;

  return w;
}

void destroy_widget(void* w)
{
  WIDGET* widget=NULL;
  if(w==NULL){
    printf("Invalid Widget, NULL\n");
    exit(-1);
  }
  widget=(WIDGET*)w;

  switch(widget->type){
  case LABEL:
    free(widget->string);
    free(widget);
    widget=NULL;
    break;
  case BUTTON:
    free(widget->string);
    free(widget);
    widget=NULL;
    break;
  }
}
