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
    printf("Widget(Label) Malloc Failed!\n");
    exit(-1);
  }

  d=malloc(strlen(message)+1);
  if(d==NULL){
    printf("Message Malloc Failed!\n");
    exit(-1);
  }
  w->flags=NONE;
  strcpy(d,message);
  w->type=LABEL;
  w->x=x;
  w->y=y;
  w->data=NULL;
  w->call=NULL;
  w->string=d;

  return w;
}

WIDGET* create_button(char* message,int x, int y,void(*ucallback)(void*),void* data)
{
  WIDGET* w=NULL;
  char* d=NULL;

  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Widget(Simple Button) Malloc Failed!\n");
    exit(-1);
  }

  d=malloc(strlen(message)+1);
  if(d==NULL){
    printf("Message Malloc Failed!\n");
    exit(-1);
  }
  strcpy(d,message);
  w->type=BUTTON;
  w->flags=CLICKABLE;
  w->x=x;
  w->y=y;
  w->data=data;
  w->call=ucallback;
  w->string=d;

  return w;
}

extern WIDGET* create_radio_button(char* message, int x, int y)
{
  WIDGET* w=NULL;
  char* d=NULL;
  int* clicked=NULL;

  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Widget(Radio Button) Malloc Failed!\n");
    exit(-1);
  }

  d=malloc(strlen(message)+1);
  if(d==NULL){
    printf("Message Malloc Failed!\n");
    exit(-1);
  }
  strcpy(d,message);

  clicked=malloc(sizeof(int));
  if(clicked==NULL){
    printf("Toggle data Malloc Failed\n");
    exit(-1);
  }
  *clicked=0;

  w->type=RADIO_BUTTON;
  w->flags=CLICKABLE;
  w->x=x;
  w->y=y;
  w->data=clicked;
  w->call=NULL;
  w->string=d;

  return w;
}

extern WIDGET* create_checkbox(char* message, int x, int y)
{
  WIDGET* w=NULL;
  char* d=NULL;
  int* clicked=NULL;

  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Widget(Checkbox) Malloc Failed!\n");
    exit(-1);
  }

  d=malloc(strlen(message)+1);
  if(d==NULL){
    printf("Message Malloc Failed!\n");
    exit(-1);
  }
  strcpy(d,message);

  clicked=malloc(sizeof(int));
  if(clicked==NULL){
    printf("Toggle data Malloc Failed\n");
    exit(-1);
  }
  *clicked=0;

  w->type=CHECKBOX;
  w->flags=CLICKABLE;
  w->x=x;
  w->y=y;
  w->data=clicked;
  w->call=NULL;
  w->string=d;

  return w;
}

extern WIDGET* create_border(int x, int y, int height, int width, int thickness)
{
  WIDGET* w=NULL;
  int* t=NULL;

  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Widget(Label) Malloc Failed!\n");
    exit(-1);
  }

  t=malloc(sizeof(int));
  if(t==NULL){
    printf("Line Thickness Failed!\n");
    exit(-1);
  }
  *t=thickness;

  w->flags=NONE;
  w->type=BORDER;
  w->x=x;
  w->y=y;
  w->height=height;
  w->width=width;
  w->data=t;
  w->call=NULL;
  w->string=NULL;

  return w;
}

extern WIDGET* create_titled_border(char* message, int x, int y, int height, int width,int thickness)
{
  WIDGET* w=NULL;
  char* d=NULL;
  int* t=NULL;

  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Widget(Titled Border) Malloc Failed!\n");
    exit(-1);
  }

  d=malloc(strlen(message)+1);
  if(d==NULL){
    printf("Message Malloc Failed!\n");
    exit(-1);
  }
  strcpy(d,message);

  t=malloc(sizeof(int));
  if(t==NULL){
    printf("Line Thickness Failed!\n");
    exit(-1);
  }
  *t=thickness;

  w->flags=NONE;
  w->type=TITLE_BORDER;
  w->x=x;
  w->y=y;
  w->height=height;
  w->width=width;
  w->data=t;
  w->call=NULL;
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
  case RADIO_BUTTON:
    free(widget->string);
    free(widget->data);
    free(widget);
    widget=NULL;
    break;
  case CHECKBOX:
    free(widget->string);
    free(widget->data);
    free(widget);
    widget=NULL;
    break;
  case BORDER:
    free(widget->data);
    free(widget);
    widget=NULL;
    break;
  case TITLE_BORDER:
    free(widget->data);
    free(widget->string);
    free(widget);
    widget=NULL;
    break;
  }
}
