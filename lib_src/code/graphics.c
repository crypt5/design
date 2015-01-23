#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <pthread.h>
#include <string.h>
#include "graphics.h"
#include "link.h"

void destroy_gui(GUI* g);
void paint_widget(GUI* g,WIDGET* w);
WIDGET* get_at_coords(GUI* g,int x, int y);
void update_mouse_down(GUI* g,WIDGET* w);

struct graphics_t{
  Display* dsp;
  Window mainWindow;
  int blackColor;
  int whiteColor;
  int bgColor;
  pthread_mutex_t lock;
  pthread_t tid;
  int run;
  Atom wm_protocols;
  Atom wm_delete_window;
  LIST* widgets;
  GC text;
  XFontStruct* font;
};

void* event_loop(void* data)
{
  GUI* g=NULL;
  XEvent e;
  int keep_running;
  WIDGET* active=NULL;
  WIDGET* clicked=NULL;

  g=(GUI*)data;
  pthread_mutex_lock(&g->lock);
  keep_running=g->run;
  pthread_mutex_unlock(&g->lock);

  while(keep_running==1){

    if(XPending(g->dsp)>0){
      XNextEvent(g->dsp,&e);
      switch (e.type) {
      case ClientMessage:
        if (e.xclient.message_type == g->wm_protocols &&
            e.xclient.data.l[0] == g->wm_delete_window)  {
	  //TODO exit code if needed
	  pthread_mutex_lock(&g->lock);
	  g->run=0;
	  pthread_mutex_unlock(&g->lock);
        }
	break;
      case ButtonPress:
	if(e.xbutton.button==1){//Left mouse Button
	  clicked=get_at_coords(g,e.xbutton.x, e.xbutton.y);
	  if(clicked!=NULL){
	    if((clicked->flags&CLICKABLE)>0){
	      active=clicked;
	      update_mouse_down(g,active);
	    }
	    else{
	      active=NULL;
	    }
	  }
	  else{
	    active=NULL;
	  }
	}
	break;
      case ButtonRelease:
	if(e.xbutton.button==1){//Left mouse Button
	  clicked=get_at_coords(g,e.xbutton.x, e.xbutton.y);
	  if(clicked!=NULL){
	    if(clicked==active){
	      active->call(active->data);
	      paint_widget(g,active);
	    }
	    else{
	      active=NULL;
	    }
	  }
	}
	break;
      default:
	printf("Unhandled Event, Code: %d\n",e.type);
	break;
      }
    }
    pthread_mutex_lock(&g->lock);
    keep_running=g->run;
    pthread_mutex_unlock(&g->lock);
  }

  return NULL;
}

GUI* init_gui()
{
  GUI* g=NULL;
  Display* d=NULL;
  int re;

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
  g->run=1;
  re=pthread_mutex_init(&g->lock,NULL);
  if(re!=0){
    printf("GUI Mutex Creation Failed\n");
    exit(-1);
  }
  g->wm_protocols = XInternAtom(g->dsp, "WM_PROTOCOLS", False);
  g->wm_delete_window = XInternAtom(g->dsp, "WM_DELETE_WINDOW", False);

  g->widgets=list_init(destroy_widget,NULL);

  g->font=XLoadQueryFont(g->dsp,"*9x15*");

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

  XSelectInput(g->dsp, w, StructureNotifyMask|KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask);

  g->mainWindow=w;
  if(title!=NULL)
    XStoreName(g->dsp,w,title);
  else
    XStoreName(g->dsp,w,"No Title");

  g->text=XCreateGC(g->dsp,g->mainWindow,0,NULL);
  XSetFont(g->dsp,g->text,g->font->fid);
  XSetForeground(g->dsp, g->text, g->blackColor);
}

void destroy_gui(GUI* g)
{
  int re;

  if(g==NULL){
    printf("GUI Never Opened\n");
    exit(-1);
  }
  pthread_mutex_lock(&g->lock);
  g->run=0;
  pthread_mutex_unlock(&g->lock);

  pthread_join(g->tid,NULL);

  XFreeFont(g->dsp,g->font);
  XFreeGC(g->dsp,g->text);
  XCloseDisplay(g->dsp);
  re=pthread_mutex_destroy(&g->lock);
  if(re!=0)
    printf("Mutex Destroy Failed\n");
  list_destroy(g->widgets);
  free(g);
  g=NULL;
}

void show_main(GUI* g)
{
  XEvent e;
  int re,i;

  if(g==NULL){
    printf("GUI Object is NULL, no wondow to show\n");
    exit(-1);
  }

  XMapWindow(g->dsp, g->mainWindow);
  XNextEvent(g->dsp,&e);
  while(e.type!=MapNotify){
    XNextEvent(g->dsp,&e);
  }
  re=pthread_create(&g->tid,NULL,event_loop,g);
  if(re!=0){
    printf("Thread Create for Event loop Failed\n");
    exit(-1);
  }
  XSetWMProtocols(g->dsp, g->mainWindow, &g->wm_delete_window, 1);

  
  for(i=0;i<list_length(g->widgets);i++){
    paint_widget(g,(WIDGET*)list_get_pos(g->widgets,i));
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

void set_main_icon(GUI* g,char* filename) //TODO
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

int gui_running(GUI* g)
{
  if(g==NULL)
    return 0;
  return g->run;
}

void add_to_main(GUI* g,WIDGET* w)
{
  if(g==NULL){
    printf("GUI is NULL, can't Add\n");
    exit(-1);
  }
  if(w==NULL){
    printf("Widget is NULL, Can't add\n");
    exit(-1);
  }
  list_add(g->widgets,w);
}

void paint_widget(GUI* g,WIDGET* w)
{
  GC gc;

  switch(w->type){
  case LABEL:
    w->width=XTextWidth(g->font,w->string,strlen(w->string));
    w->height=g->font->ascent;
    XDrawString(g->dsp,g->mainWindow,g->text,w->x,w->y+w->height,(char*)w->string,strlen((char*)w->string));
    break;
  case BUTTON:
    w->width=XTextWidth(g->font,w->string,strlen(w->string))+20;
    w->height=g->font->ascent;
    gc=XCreateGC(g->dsp,g->mainWindow,0,NULL);
    XSetForeground(g->dsp,gc,0x00AAAAAA);
    XFillRectangle(g->dsp,g->mainWindow,gc,w->x,w->y,w->width,w->height*2);
    XDrawString(g->dsp,g->mainWindow,g->text,w->x+10,w->y+(w->height+w->height/2),(char*)w->string,strlen((char*)w->string));
    w->height=w->height*2;
    XFreeGC(g->dsp,gc);
    break;
  }
}

WIDGET* get_at_coords(GUI* g,int x, int y)
{
  int i;
  WIDGET* temp=NULL;

  for(i=0;i<list_length(g->widgets);i++){
    temp=list_get_pos(g->widgets,i);
    if(x>temp->x&&x<temp->x+temp->width){
      if(y>temp->y&&y<temp->y+temp->height){
	return temp;
      }
    }
  }
  return NULL;
}

void update_mouse_down(GUI* g,WIDGET* w)
{
  //TODO Update Graphics Code
  GC gc;
  gc=XCreateGC(g->dsp,g->mainWindow,0,NULL);
  XFillRectangle(g->dsp,g->mainWindow,gc,w->x,w->y,w->width,w->height);
  XFreeGC(g->dsp,gc);
}

