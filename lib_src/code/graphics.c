#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <pthread.h>
#include "graphics.h"

struct graphics_t{
  Display* dsp;
  Window mainWindow;
  // Add more stuff here as needed
};

GUI* init_gui()
{
  GUI* g=NULL;
  Display* d=NULL;

  g=malloc(sizeof(GUI));
  if(g==NULL){
    printf("Main GUI struct failed!\n");
    exit(-1);
  }

  d=XOpenDisplay(NULL);
  if(d==NULL){
    printf("Could not open display\n");
    exit(-1);
  }

  g->dsp=d;

  //Create everything needed here for convenience

  return g;
}

void create_main_window(GUI* g,char* title,int mode)
{
  Window w;

  int blackColor = BlackPixel(g->dsp, DefaultScreen(g->dsp));
  int whiteColor = WhitePixel(g->dsp, DefaultScreen(g->dsp));

  if(g==NULL){
    printf("Can't create window, GUI is NULL\n");
    exit(-1);
  }

  w = XCreateSimpleWindow(g->dsp,DefaultRootWindow(g->dsp),0,0,200,100,0,blackColor, blackColor);

  XSelectInput(g->dsp, w, StructureNotifyMask);

  g->mainWindow=w;
  if(title!=NULL)
    XStoreName(g->dsp,w,title);
}

void destroy_gui(GUI* g)
{
  if(g==NULL){
    printf("GUI object is NULL, can't destroy\n");
    exit(-1);
  }

  XCloseDisplay(g->dsp);
  free(g);
  g=NULL;
}

void show_main(GUI* g)
{
 XEvent e;

  if(g==NULL){
    printf("GUI Object is NULL, no wondow to show\n");
    exit(-1);
  }

  XMapWindow(g->dsp, g->mainWindow);
  XNextEvent(g->dsp,&e);
  while(e.type!=MapNotify){
    XNextEvent(g->dsp,&e);
  }

}

void set_main_size(GUI* g,int height, int width)
{
  if(g==NULL){
    printf("GUI Object NULL, No window\n");
    exit(-1);
  }

  XResizeWindow(g->dsp,g->mainWindow,width,height);

}
