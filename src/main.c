#include <stdio.h>
#include <unistd.h>
#include "graphics.h"

int main()
{
  GUI* g=NULL;
  g=init_gui();
  set_main_background(g,0x00000000);
  set_main_icon(g,"makefile");
  create_main_window(g,"test window");
  set_main_size(g,600,1000);
  sleep(2);
  show_main(g);
  sleep(10);
  destroy_gui(g);
  return 0;
}
