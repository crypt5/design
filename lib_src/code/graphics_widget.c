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
  w->data=d;

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
    free(widget->data);
    free(widget);
    widget=NULL;
    break;
  }
}

int comp_widget(void* w1,void* w2)
{
  //TODO
  return 1;
}
