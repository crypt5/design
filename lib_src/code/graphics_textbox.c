#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_textbox.h"

struct textbox_data_t{
  int max_length;
  int current_pos;
};

void paint_textbox(GUI* g,WIDGET* w)
{
  int width;
  struct textbox_data_t* data=NULL;
  if(w->width==0){
    data=w->widget_data;
    for(width=0;width<data->max_length;width++){
      w->string[width]=' ';
    }
    w->width=XTextWidth(g->font,w->string,strlen(w->string))+8;
    w->height=g->font->ascent*2;
    for(width=0;width<data->max_length;width++){
      w->string[width]='\0';
    }
  }
  XSetForeground(g->dsp,g->draw,g->whiteColor);
  XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
  XSetForeground(g->dsp,g->draw,g->blackColor);
  XDrawRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
  XDrawString(g->dsp,g->mainWindow,g->text,w->x+4,w->y+w->height/2+w->height/4,w->string,strlen(w->string));
}

void paint_textbox_click(GUI* g, WIDGET* w)
{
  int i;
  struct textbox_data_t* data=NULL;
  data=w->widget_data;
  i=XTextWidth(g->font,"A",1);
  XDrawString(g->dsp,g->mainWindow,g->text,w->x+(i*data->current_pos),w->y+w->height/2+w->height/4,"|",1);
}

void update_textbox(GUI* g,WIDGET* w, char c)
{
  struct textbox_data_t* data=w->widget_data;
  int i;
  if(c>=' '&&strlen(w->string)<data->max_length){
    if(w->string[data->current_pos]=='\0'){
      w->string[data->current_pos]=c;
    }
    else{
      for(i=data->max_length;i>data->current_pos;i--)
	w->string[i]=w->string[i-1];
      w->string[data->current_pos]=c;
    }
    data->current_pos++;
  }
  else if(c<' '){
    switch(c){
    case 0:
      //No Operation return from key parsing function
      break;
    case BACKSPACE:
      if(data->current_pos>0){
	if(w->string[data->current_pos]=='\0'){
	  data->current_pos--;
	  w->string[data->current_pos]='\0';
	}
	else{
	  data->current_pos--;
	  for(i=data->current_pos;i<data->max_length;i++)
	    w->string[i]=w->string[i+1];
	}
      }
      break;
    case DELETE:
      if(data->current_pos<data->max_length&&w->string[data->current_pos]!='\0'){
	for(i=data->current_pos;i<data->max_length;i++)
	  w->string[i]=w->string[i+1];
      }
      break;
    case LEFT:
      if(data->current_pos>0)
	data->current_pos--;
      break;
    case RIGHT:
      if(data->current_pos<data->max_length&&w->string[data->current_pos]!='\0')
	data->current_pos++;
      break;
    case ENTER:
    case UP:
    case DOWN:
    case TAB:
      //We dont care in a textbox
      break;
    default:
      printf("Unknown Control Operator: %d\n",c);
    }
  }
  paint_textbox(g,w);
  paint_textbox_click(g,w);
}

WIDGET* create_textbox(int x,int y,int max_length)
{
  WIDGET* w=NULL;
  struct textbox_data_t* data=NULL;
  char* s=NULL;
  int i;

  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Widget malloc failed!\n");
    exit(-1);
  }
  data=malloc(sizeof(struct textbox_data_t));
  if(data==NULL){
    printf("Textbox data malloc failed!\n");
    exit(-1);
  }
  data->current_pos=0;
  data->max_length=max_length;
  s=malloc(max_length+1);
  if(s==NULL){
    printf("Textbox string malloc failed\n");
    exit(-1);
  }
  for(i=0;i<=max_length;i++)
    s[i]='\0';

  w->type=TEXTBOX;
  w->flags=CLICKABLE|SELECTABLE;
  w->enable=1;
  w->x=x;
  w->y=y;
  w->height=0;
  w->width=0;
  w->call=NULL;
  w->paint=paint_textbox;
  w->click=paint_textbox_click;
  w->select=NULL;
  w->key_press=update_textbox;
  w->ufree=destroy_textbox;
  w->string=s;
  w->data=NULL;
  w->widget_data=data;

  return w;
}

void destroy_textbox(WIDGET* w)
{
  if(w==NULL){
    printf("Widgetis NULL!\n");
    exit(-2);
  }
  if(w->type!=TEXTBOX){
    printf("Widget not a Textbox\n");
    exit(-2);
  }
  free(w->string);
  free(w->widget_data);
}
