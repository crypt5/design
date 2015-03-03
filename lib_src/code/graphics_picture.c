#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include "graphics.h"
#include "graphics_widget.h"
#include "graphics_picture.h"

struct picture_data_t{
  Pixmap img;
};

void paint_picture(GUI* g, WIDGET *w)
{
  struct picture_data_t* data=w->widget_data;

}

WIDGET* create_picture(GUI* g,char* filename,int x,int y)
{
  WIDGET* w=NULL;
  struct picture_data_t* data=NULL;
  unsigned int height,width;

  w=malloc(sizeof(WIDGET));
  if(w==NULL){
    printf("Initial Widget Malloc Failed!!\n");
    exit(-1);
  }
  data=malloc(sizeof(struct picture_data_t));
  if(data==NULL){
    printf("Data malloc Failed!!!\n");
    exit(-1);
  }

  if(strstr(filename, ".xpm") != NULL){
    if(access(filename,R_OK)!=-1){
      XReadBitmapFile(g->dsp,g->mainWindow,filename,&width,&height,&data->img,0,0);
    }
  }

  w->type=PICTURE;
  w->flags=NONE;
  w->enable=1;
  w->visible=1;
  w->x=x;
  w->y=y;
  w->height=(int)height;
  w->width=(int)width;
  w->call=NULL;
  w->paint=paint_picture;
  w->select=NULL;
  w->key_press=NULL;
  w->ufree=destroy_picture;
  w->string=NULL;
  w->data=g;
  w->widget_data=data;

  return w;
}

void destroy_picture(WIDGET* w)
{
  GUI* g=NULL;
  struct picture_data_t* data=NULL;

  if(w==NULL){
    printf("WIDGET is NULL!!!\n");
    exit(-1);
  }
  if(w->type!=PICTURE){
    printf("WIDGET is not a picture!\n");
    exit(-1);
  }
  g=w->data;
  data=w->widget_data;
  XFreePixmap(g->dsp,data->img);
  free(data);
  free(w);
}

