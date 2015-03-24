#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_button.h"


void paint_button(GUI* g, WIDGET* w)
{
  if(w->width!=0){
    XSetForeground(g->dsp,g->draw,g->bgColor);
    XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
  }
  w->width=XTextWidth(g->font,w->string,strlen(w->string))+20;
  w->height=g->font->ascent*2;
  if((w->status&STATUS_VISIBLE)==0){
    printf("NOT VISIBLE\nSTATUS: %d\n",w->status);
    XSetForeground(g->dsp,g->draw,g->bgColor);
    XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
    return;
  }
  XSetForeground(g->dsp,g->draw,(w->status&STATUS_ENABLE)>0 ? 0x00AAAAAA : 0x00CCCCCC);
  XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
  if(*(int*)w->widget_data>0){
    XSetForeground(g->dsp,g->text,(w->status&STATUS_ENABLE)>0 ? *(int*)w->widget_data : to_gray(*(int*)w->widget_data));
    XDrawString(g->dsp,g->mainWindow,g->text,w->x+10,w->y+(w->height/2+w->height/4),w->string,strlen(w->string));
    XSetForeground(g->dsp,g->text,g->blackColor);
  }
  else{
    if((w->status&STATUS_ENABLE)==0){
      XSetForeground(g->dsp,g->text,0x00EEEEEE);
      XDrawString(g->dsp,g->mainWindow,g->text,w->x+10,w->y+(w->height/2+w->height/4),w->string,strlen(w->string));
      XSetForeground(g->dsp,g->text,g->blackColor);
    }
    else {
      XDrawString(g->dsp,g->mainWindow,g->text,w->x+10,w->y+(w->height/2+w->height/4),w->string,strlen(w->string));
    }
  }
}

void paint_click(GUI* g, WIDGET* w)
{
  XSetForeground(g->dsp,g->draw,0x00808080);
  XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
  if(*(int*)w->widget_data<0){
    XDrawString(g->dsp,g->mainWindow,g->text,w->x+10,w->y+(w->height/2+w->height/4),w->string,strlen(w->string));
  }
  else {
    XSetForeground(g->dsp,g->text,*(int*)w->widget_data);
    XDrawString(g->dsp,g->mainWindow,g->text,w->x+10,w->y+(w->height/2+w->height/4),w->string,strlen(w->string));
    XSetForeground(g->dsp,g->text,g->blackColor);
  }
}

WIDGET* create_button(char* text, int x, int y)
{
  WIDGET* w=NULL;
  char* d=NULL;
  int* color=NULL;
  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Button Malloc failed!\n");
    exit(-1);
  }
  d=malloc(strlen(text)+1);
  if(d==NULL){
    printf("String malloc failed\n");
    exit(-1);
  }
  color=malloc(sizeof(int));
  if(color==NULL){
    printf("Text color malloc failed\n");
    exit(-1);
  }
  *color=-1;
  strcpy(d,text);
  w->type=BUTTON;
  w->flags=CLICKABLE;
  w->status=STATUS_VISIBLE|STATUS_ENABLE;
  w->x=x;
  w->y=y;
  w->height=0;
  w->width=0;
  w->call=NULL;
  w->paint=paint_button;
  w->click=paint_click;
  w->select=NULL;
  w->key_press=NULL;
  w->ufree=destroy_button;
  w->string=d;
  w->data=NULL;
  w->widget_data=color;

  return w;
}

void destroy_button(GUI* g,WIDGET* w)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  free(w->widget_data);
  free(w->string);
  free(w);
}

void set_button_callback(WIDGET* w,void(*ucallback)(GUI* g,WIDGET* self, void* data),void* data)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  w->call=ucallback;
  w->data=data;

}

void set_button_paint_select(WIDGET* w,void(*uselect)(GUI* g, WIDGET* w),void(*ukey_press)(GUI* g,WIDGET* w,char key))
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  w->select=uselect;
  w->type=w->type|SELECTABLE;
  w->key_press=ukey_press;
}

void remove_button_click_callback(WIDGET* w)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  w->call=NULL;
  w->data=NULL;
}

void remove_button_paint_select(WIDGET* w)
{
 if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  w->select=NULL;
  w->key_press=NULL;
  w->flags=w->flags&(0xFF^SELECTABLE);
}

void set_button_text_color(WIDGET* w,int ARGB)
{
 if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  *(int*)w->widget_data=ARGB;
}

void set_button_text(WIDGET* w, char* message)
{
  char* s=NULL;
 if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  free(w->string);
  s=malloc(strlen(message)+1);
  if(s==NULL){
    printf("String re malloc failed!\n");
    exit(-1);
  }
  strcpy(s,message);
  w->string=s;
}

void set_button_enable(WIDGET* w,int enable)
{
 if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  if(enable==1)
    w->status=w->status|STATUS_ENABLE;
  else if(enable==0)
    w->status=w->status&~STATUS_ENABLE;
  else
    printf("Invalid value for enable\nNo action taken\n");
}

void set_button_visible(WIDGET* w,int visible)
{
 if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  if(visible==1)
    w->status=w->status|STATUS_VISIBLE;
  else if(visible==0)
    w->status=w->status&~STATUS_VISIBLE;
  else
    printf("Invalid value for visible\nNo action taken\n");
}

int get_button_text_color(WIDGET* w)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  return *(int*)w->widget_data;
}

char* get_button_text(WIDGET* w)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  return w->string;
}

int get_button_enable(WIDGET* w)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  if((w->status&STATUS_ENABLE)>0)
    return 1;
  else
    return 0;
}

int get_button_visible(WIDGET* w)
{
  if(w==NULL){
    printf("Widget is NULL\n");
    exit(-2);
  }
  if(w->type!=BUTTON){
    printf("Widget not a button!\n");
    exit(-2);
  }
  if((w->status&STATUS_VISIBLE)>0)
    return 1;
  else
    return 0;
}
