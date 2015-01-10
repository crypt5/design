#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <pthread.h>
#include "graphics.h"

struct graphics_t{
  Display* dsp;
  Window* mainWindow;
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
  g->mainWindow=NULL;

  //Create everything needed here for convenience

  return g;
}

void create_main_window(GUI* g)
{
  Window w;
  XEvent e;

  int blackColor = BlackPixel(g->dsp, DefaultScreen(g->dsp));
  int whiteColor = WhitePixel(g->dsp, DefaultScreen(g->dsp));

  if(g==NULL){
    printf("Can't create window, GUI is NULL\n");
    exit(-1);
  }

  w = XCreateSimpleWindow(g->dsp,DefaultRootWindow(g->dsp),0,0,200,100,0,blackColor, blackColor);

  XSelectInput(g->dsp, w, StructureNotifyMask);
  XMapWindow(g->dsp, w);

  g->mainWindow=&w;

  XNextEvent(g->dsp,&e);
  while(e.type!=MapNotify){
    XNextEvent(g->dsp,&e);
  }

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
