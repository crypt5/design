#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <pthread.h>
#include "graphics.h"

struct graphics_t{
  Display* dsp;
  Window mainWindow;
  int blackColor;
  int whiteColor;
  int bgColor;

  //Need Widget List

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
  g->blackColor = BlackPixel(g->dsp, DefaultScreen(g->dsp));
  g->whiteColor = BlackPixel(g->dsp, DefaultScreen(g->dsp));
  g->bgColor=0x00AD855C;

  //Create everything needed here for convenience

  return g;
}

void create_main_window(GUI* g,char* title)
{
  Window w;

  if(g==NULL){
    printf("Can't create window, GUI is NULL\n");
    exit(-1);
  }

  w = XCreateSimpleWindow(g->dsp,DefaultRootWindow(g->dsp),0,0,200,100,0,g->bgColor,g->bgColor);

  XSelectInput(g->dsp, w, StructureNotifyMask);

  g->mainWindow=w;
  if(title!=NULL)
    XStoreName(g->dsp,w,title);
  else
    XStoreName(g->dsp,w,"No Title");
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

void set_main_background(GUI* g,int RGB)
{
  if(g==NULL){
    printf("GUI Is NULL, No BG color to change\n");
    exit(-1);
  }

  g->bgColor=RGB;
}

void set_main_icon(GUI* g,char* filename)
{
  XWMHints* hint=NULL;
  if(g==NULL){
    printf("Can't set Icon, GUI is NULL\n");
    exit(-1);
  }

  if(access(filename,R_OK)!=-1){
    hint=XAllocWMHints();
    //File exists, create icon pixmap

    XFree(hint);
  }
  else {
    printf("Invalid File, Default icon used\n");
  }
}
