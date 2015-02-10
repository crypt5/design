#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <pthread.h>
#include <string.h>
#include "graphics.h"
#include "link.h"

#define BACKSPACE 1
#define TAB 2
#define ENTER 3
#define LEFT 4
#define RIGHT 5
#define UP 6
#define DOWN 7
#define DELETE 8

void destroy_gui(GUI* g);
void paint_widget(GUI* g,WIDGET* w);
WIDGET* get_at_coords(GUI* g,int x, int y);
void update_mouse_down(GUI* g,WIDGET* w);
char process_keystroke(GUI* g, XKeyEvent* e);
void update_textbox(char c,GUI* g, WIDGET* w);

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
  GC draw;
  XFontStruct* font;
};

void* event_loop(void* data)
{
  GUI* g=NULL;
  XEvent e;
  int keep_running,i;
  WIDGET* active=NULL;
  WIDGET* clicked=NULL;
  WIDGET* selected=NULL;
  char key;

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
	    if(clicked!=selected&&selected!=NULL){
	      paint_widget(g,selected);
	      selected=NULL;
	    }
	    if((clicked->flags&(CLICKABLE|SELECTABLE))>0){
	      switch(clicked->type){
	      case BUTTON:
		active=clicked;
		update_mouse_down(g,active);
		break;
	      case RADIO_BUTTON:
		active=clicked;
		if(*(int*)active->data==1)
		  *(int*)active->data=0;
		else
		  *(int*)active->data=1;
		update_mouse_down(g,active);
		break;
	      case CHECKBOX:
		active=clicked;
		if(*(int*)active->data==1)
		  *(int*)active->data=0;
		else
		  *(int*)active->data=1;
		update_mouse_down(g,active);
		break;
	      case TEXTBOX:
		active=clicked;
		break;
	      }
	    }
	    else{
	      active=NULL;
	    }
	  }
	  else{
	    active=NULL;
	    if(selected!=NULL)
	      paint_widget(g,selected);
	    selected=NULL;
	  }
	}
	break;
      case ButtonRelease:
	if(e.xbutton.button==1){//Left mouse Button
	  clicked=get_at_coords(g,e.xbutton.x, e.xbutton.y);
	  if(clicked!=NULL){
	    if(clicked==active){
	      if((clicked->flags&(CLICKABLE|SELECTABLE))>0){
		switch(clicked->type){
		case BUTTON:
		  if(active->call!=NULL)
		    active->call(active->data);
		  paint_widget(g,active);
		  break;
		case RADIO_BUTTON:
		  paint_widget(g,active);
		  break;
		case CHECKBOX:
		  paint_widget(g,active);
		  break;
		case TEXTBOX:
		  selected=active;
		  update_mouse_down(g,active);
		  break;
		}
	      }
	    }
	    else{
	      if(active!=NULL){
		paint_widget(g,active);
	      }
	      active=NULL;
	    }
	  }
	  else{
	    if(active!=NULL)
	      paint_widget(g,active);
	    active=NULL;
	  }
	}
	break;
      case KeyRelease:
	if(selected!=NULL){
	  key=process_keystroke(g,&e.xkey);
	  update_textbox(key,g,selected);
	}
	break;
      case Expose://Parts or whole window is visible again
      case MapNotify: 
	//TODO Smart Repainting Code
	for(i=0;i<list_length(g->widgets);i++){
	  paint_widget(g,(WIDGET*)list_get_pos(g->widgets,i));
	}
	break;
      case UnmapNotify:
      case ConfigureNotify:
	// Ignore these events
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
  g->whiteColor = WhitePixel(g->dsp, DefaultScreen(g->dsp));
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

  XSelectInput(g->dsp, w, StructureNotifyMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|ExposureMask);

  g->mainWindow=w;
  if(title!=NULL)
    XStoreName(g->dsp,w,title);
  else
    XStoreName(g->dsp,w,"No Title");

  g->text=XCreateGC(g->dsp,g->mainWindow,0,NULL);
  g->draw=XCreateGC(g->dsp,g->mainWindow,0,NULL);
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
  XFreeGC(g->dsp,g->draw);
  XCloseDisplay(g->dsp);
  re=pthread_mutex_destroy(&g->lock);
  if(re!=0)
    printf("Mutex Destroy Failed\n");
  list_destroy(g->widgets);
  free(g);
  g=NULL;
}

void shutdown_gui(GUI* g)
{
  pthread_mutex_lock(&g->lock);
  g->run=0;
  pthread_mutex_unlock(&g->lock);
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

void update_widget(GUI* g, WIDGET* w)
{
  paint_widget(g,w);
}

void paint_widget(GUI* g,WIDGET* w)
{
  int width;
  struct textbox_data_t* data=NULL;
  switch(w->type){
  case LABEL:
    w->width=XTextWidth(g->font,w->string,strlen(w->string));
    w->height=g->font->ascent;
    XSetForeground(g->dsp,g->draw,g->bgColor);
    XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
    XDrawString(g->dsp,g->mainWindow,g->text,w->x,w->y+w->height,(char*)w->string,strlen((char*)w->string));
    break;
  case BUTTON:
    w->width=XTextWidth(g->font,w->string,strlen(w->string))+20;
    w->height=g->font->ascent;
    XSetForeground(g->dsp,g->draw,0x00AAAAAA);
    XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height*2);
    XDrawString(g->dsp,g->mainWindow,g->text,w->x+10,w->y+(w->height+w->height/2),(char*)w->string,strlen((char*)w->string));
    w->height=w->height*2;
    break;
  case RADIO_BUTTON:
    w->height=g->font->ascent*2;
    w->width=XTextWidth(g->font,w->string,strlen(w->string))+30;
    XSetForeground(g->dsp,g->draw,g->whiteColor);
    XFillArc(g->dsp,g->mainWindow,g->draw,w->x+3,w->y+(g->font->ascent/6),w->height-5,w->height-5,0,360*64);
    XSetForeground(g->dsp,g->draw,g->blackColor);
    XDrawArc(g->dsp,g->mainWindow,g->draw,w->x+3,w->y+(g->font->ascent/6),w->height-5,w->height-5,0,360*64);
    XDrawString(g->dsp,g->mainWindow,g->text,w->x+28,w->y+w->height-(g->font->ascent/2),(char*)w->string,strlen((char*)w->string));

    if(*(int*)w->data==1){
      XSetForeground(g->dsp,g->draw,0x000000AA);
      XFillArc(g->dsp,g->mainWindow,g->draw,w->x+4+(w->height-5)/4,w->y+(g->font->ascent/2),10,10,0,360*64);
    }
    break;
  case CHECKBOX:
    w->height=g->font->ascent*2;
    w->width=XTextWidth(g->font,w->string,strlen(w->string))+30;
    XSetForeground(g->dsp,g->draw,g->whiteColor);
    XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x+5,w->y+(w->height/5),15,15);
    XSetForeground(g->dsp,g->draw,g->blackColor);
    XDrawRectangle(g->dsp,g->mainWindow,g->draw,w->x+5,w->y+(w->height/5),15,15);
    XDrawString(g->dsp,g->mainWindow,g->text,w->x+28,w->y+w->height-(g->font->ascent/2),(char*)w->string,strlen((char*)w->string));

    if(*(int*)w->data==1){
      XSetForeground(g->dsp,g->draw,0x0000AA00);
      //Short Leg
      XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+9,w->x+12,w->y+14);
      XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+10,w->x+12,w->y+15);
      XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+11,w->x+12,w->y+16);
      XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+12,w->x+12,w->y+17);
      //Long Leg
      XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+12,w->y+14,w->x+17,w->y+5);
      XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+12,w->y+15,w->x+17,w->y+6);
      XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+12,w->y+16,w->x+17,w->y+7);
      XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+12,w->y+17,w->x+17,w->y+8);
    }
    break;
  case BORDER:
    XSetForeground(g->dsp,g->draw,g->blackColor);
    XSetLineAttributes(g->dsp,g->draw,*(int*)w->data,LineSolid,CapButt,JoinMiter);
    XDrawRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
    XSetLineAttributes(g->dsp,g->draw,0,LineSolid,CapButt,JoinMiter);
    break;
  case TITLE_BORDER:
    XSetForeground(g->dsp,g->draw,g->blackColor);
    width=XTextWidth(g->font,w->string,strlen(w->string))+2;
    XSetLineAttributes(g->dsp,g->draw,*(int*)w->data,LineSolid,CapButt,JoinMiter);
    XDrawRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
    XSetLineAttributes(g->dsp,g->draw,0,LineSolid,CapButt,JoinMiter);
    XSetForeground(g->dsp,g->draw,g->bgColor);
    XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x+5,w->y-(*(int*)w->data/2),width,*(int*)w->data);
    XDrawString(g->dsp,g->mainWindow,g->text,w->x+6,w->y+(g->font->ascent/2),(char*)w->string,strlen((char*)w->string));
    break;
  case TEXTBOX:
    if(w->width==0){
      data=w->data;
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
    XDrawString(g->dsp,g->mainWindow,g->text,w->x+4,w->y+w->height/2+w->height/4,(char*)w->string,strlen((char*)w->string));
    break;
  }
}

WIDGET* get_at_coords(GUI* g,int x, int y)
{
  int i;
  WIDGET* temp=NULL;

  for(i=0;i<list_length(g->widgets);i++){
    temp=list_get_pos(g->widgets,i);
    if(temp->type!=BORDER&&temp->type!=TITLE_BORDER){
      if(x>temp->x&&x<temp->x+temp->width){
	if(y>temp->y&&y<temp->y+temp->height){
	  return temp;
	}
      }
    }
  }
  return NULL;
}

void update_mouse_down(GUI* g,WIDGET* w)
{
  int i;
  struct textbox_data_t* data=NULL;
  switch(w->type){
  case BUTTON:
    XSetForeground(g->dsp,g->draw,0);
    XFillRectangle(g->dsp,g->mainWindow,g->draw,w->x,w->y,w->width,w->height);
    break;
  case RADIO_BUTTON:
    if(*(int*)w->data==1){
      XSetForeground(g->dsp,g->draw,0x000000AA);
      XFillArc(g->dsp,g->mainWindow,g->draw,w->x+4+(w->height-5)/4,w->y+(g->font->ascent/2),10,10,0,360*64);
    }
    break;
  case CHECKBOX:
    if(*(int*)w->data==1){
      XSetForeground(g->dsp,g->draw,0x0000AA00);
      //Short Leg
      XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+9,w->x+12,w->y+14);
      XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+10,w->x+12,w->y+15);
      XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+11,w->x+12,w->y+16);
      XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+7,w->y+12,w->x+12,w->y+17);
      //Long Leg
      XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+12,w->y+14,w->x+17,w->y+5);
      XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+12,w->y+15,w->x+17,w->y+6);
      XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+12,w->y+16,w->x+17,w->y+7);
      XDrawLine(g->dsp,g->mainWindow,g->draw,w->x+12,w->y+17,w->x+17,w->y+8);
    }
    break;
  case TEXTBOX:
    data=w->data;
    i=XTextWidth(g->font,"A",1);
    XDrawString(g->dsp,g->mainWindow,g->text,w->x+(i*data->current_pos),w->y+w->height/2+w->height/4,"|",1);
    break;
  }
}

char process_keystroke(GUI* g, XKeyEvent* e)
{
  char re=0;
  KeySym key_symbol=XkbKeycodeToKeysym(g->dsp, e->keycode, 0, e->state & ShiftMask ? 1 : 0);
  if(key_symbol>=XK_space && key_symbol<=XK_asciitilde){
    re=key_symbol - XK_space + ' ';
  }
  else{
    switch(key_symbol){
    case XK_KP_Insert:
      re='0';
      break;
    case XK_KP_End:
      re='1';
      break;
    case XK_KP_Down:
      re='2';
      break;
    case XK_KP_Page_Down:
      re='3';
      break;
    case XK_KP_Left:
      re='4';
      break;
    case XK_KP_Begin:
      re='5';
      break;
    case XK_KP_Right:
      re='6';
      break;
    case XK_KP_Home:
      re='7';
      break;
    case XK_KP_Up:
      re='8';
      break;
    case XK_KP_Page_Up:
      re='9';
      break;
   case XK_KP_Delete:
      re='.';
      break;
    case XK_KP_Add:
      re='+';
      break;
    case XK_KP_Subtract:
      re='-';
      break;
    case XK_KP_Multiply:
      re='*';
      break;
    case XK_KP_Divide:
      re='/';
      break;
    case XK_BackSpace:
      re=BACKSPACE;
      break;
    case XK_Tab:
      re=TAB;
      break;
    case XK_Return:
    case XK_KP_Enter:
      re=ENTER;
      break;
    case XK_Left:
      re=LEFT;
      break;
    case XK_Right:
      re=RIGHT;
      break;
    case XK_Up:
      re=UP;
      break;
    case XK_Down:
      re=DOWN;
      break;
    case XK_Delete:
      re=DELETE;
      break;
      /*default: //Upmapped Key we dont care about
	printf("Key still Unbound\n");*/
    }
  }
  return re;
}

void update_textbox(char c,GUI* g, WIDGET* w)
{
  struct textbox_data_t* data=w->data;
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
  paint_widget(g,w);
  update_mouse_down(g,w);
}
