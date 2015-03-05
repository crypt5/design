#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_combobox.h"

typedef char*(*ustring)(void* uitem);
struct combobox_data_t{
  int text_color;
  int background_color;
  Pixmap hidden;
  int dropdown;
  int max_display;
  int number_items;
  int max_length;
  int selected;
  void** items;
  ustring get_string;
};

void paint_combobox(GUI* g,WIDGET* w)
{
  printf("Need painting Code\n");
  //TODO Painting Code
}

void paint_combobox_clicked(GUI* g, WIDGET* w)
{
  printf("Combobox Clicked!\n");
  //TODO Painting Code
}

WIDGET* create_combobox(int display_width,int x,int y,char*(*ustring)(void* uitem))
{
  WIDGET* w=NULL;
  char* s=NULL;
  struct combobox_data_t* data=NULL;

  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Combobox Malloc Failed!\n");
    exit(-1);
  }
  data=malloc(sizeof(struct combobox_data_t));
  if(data==NULL){
    printf("ComboBox data mallco Failed!\n");
    exit(-1);
  }
  s=malloc(display_width);
  if(s==NULL){
    printf("Combobox Display String malloc Failed\n");
    exit(-1);
  }

  data->text_color=-1;
  data->background_color=-1;
  data->dropdown=0;
  data->max_display=10;
  data->number_items=0;
  data->max_length=display_width;
  data->selected=-1;
  data->items=NULL;
  data->get_string=ustring;

  w->type=COMBOBOX;
  w->flags=CLICKABLE;
  w->enable=1;
  w->visible=1;
  w->x=x;
  w->y=y;
  w->height=0;
  w->width=0;
  w->call=NULL;
  w->paint=paint_combobox;
  w->click=paint_combobox_clicked;
  w->select=NULL;
  w->key_press=NULL;
  w->ufree=destroy_combobox;
  w->string=s;
  w->data=NULL;
  w->widget_data=data;

  return w;
}

void destroy_combobox(WIDGET* w)
{
  struct combobox_data_t* data=NULL;
  if(w==NULL){
    printf("WIDGET is null!\n");
    exit(-1);
  }
  if(w->type!=COMBOBOX){
    printf("Not a Combobox!\n");
    exit(-1);
  }
  data=w->widget_data;
  if(data->items!=NULL)
    free(data->items);
  free(w->string);
  free(w->widget_data);
  free(w);

}

void add_item_to_combobox(WIDGET* w,void* item)
{
  struct combobox_data_t* data=NULL;
  if(w==NULL){
    printf("WIDGET is null!\n");
    exit(-1);
  }
  if(w->type!=COMBOBOX){
    printf("Not a Combobox!\n");
    exit(-1);
  }
  data=w->widget_data;
  if(data->number_items==0){
    data->items=malloc(sizeof(void*));
    if(data->items==NULL){
      printf("Initial Item Malloc failed!\n");
      exit(-1);
    }
    data->items[0]=item;
    data->number_items++;
  }
  else{
    data->items=realloc(data->items,(data->number_items+1)*sizeof(void*));
    if(data->items==NULL){
      printf("Item Realloc failed!\n");
      exit(-1);
    }
    data->items[data->number_items]=item;
   data->number_items++;
  }
  
}

void* remove_item_from_combobox(WIDGET* w,int index)
{
  void* re=NULL;
  int i;
  struct combobox_data_t* data=NULL;
  if(w==NULL){
    printf("WIDGET is null!\n");
    exit(-1);
  }
  if(w->type!=COMBOBOX){
    printf("Not a Combobox!\n");
    exit(-1);
  }
  data=w->widget_data;
  if(data->number_items>0&&index<=data->number_items){
    if(data->number_items>1){
      re=data->items[index];
      for(i=index;i<data->number_items-1;i++){
	data->items[i]=data->items[i+1];
	data->items=realloc(data->items,(data->number_items-1)*sizeof(void*));
	if(data->items==NULL){
	  printf("Item list realloc failed!\n");
	  exit(-1);
	}
	data->number_items--;
      }
    }
    else{
      re=data->items[0];
      data->number_items=0;
      free(data->items);
      data->items=NULL;
    }
  }
  return re;
}

int get_number_of_items_in_combobox(WIDGET* w)
{
  struct combobox_data_t* data=NULL;
  if(w==NULL){
    printf("WIDGET is null!\n");
    exit(-1);
  }
  if(w->type!=COMBOBOX){
    printf("Not a Combobox!\n");
    exit(-1);
  }
  data=w->widget_data;
  return data->number_items;
}

void set_combobox_number_to_display(WIDGET* w,int display)
{
  struct combobox_data_t* data=NULL;
  if(w==NULL){
    printf("WIDGET is null!\n");
    exit(-1);
  }
  if(w->type!=COMBOBOX){
    printf("Not a Combobox!\n");
    exit(-1);
  }
  data=w->widget_data;
  data->max_display=display;
}

void set_combobox_background_color(WIDGET* w,int ARGB)
{
  struct combobox_data_t* data=NULL;
  if(w==NULL){
    printf("WIDGET is null!\n");
    exit(-1);
  }
  if(w->type!=COMBOBOX){
    printf("Not a Combobox!\n");
    exit(-1);
  }
  data=w->widget_data;
  data->background_color=ARGB;
}

void set_combobox_text_color(WIDGET* w,int ARGB)
{
  struct combobox_data_t* data=NULL;
  if(w==NULL){
    printf("WIDGET is null!\n");
    exit(-1);
  }
  if(w->type!=COMBOBOX){
    printf("Not a Combobox!\n");
    exit(-1);
  }
  data=w->widget_data;
  data->text_color=ARGB;
}

void set_combobox_selected_item(WIDGET* w,int select)
{
  struct combobox_data_t* data=NULL;
  if(w==NULL){
    printf("WIDGET is null!\n");
    exit(-1);
  }
  if(w->type!=COMBOBOX){
    printf("Not a Combobox!\n");
    exit(-1);
  }
  data=w->widget_data;
  if(select<=data->number_items)
    data->selected=select;
}
