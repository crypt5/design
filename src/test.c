#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "graphics.h"

char* printer(void* data)
{
  return (char*)data;
}

int main()
{
  GUI* g=NULL;
  WIDGET* label=NULL;

  g=init_gui();
  create_main_window(g,"Widget Testing");
  set_main_size(g,500,500);

  label=create_label("Hello World!",10,10);

  add_to_main(g,label);

  show_main(g);
  while(gui_running(g)){
    usleep(100);
  }
  destroy_gui(g);
}
