#include <stdio.h>
#include <unistd.h>
#include "graphics.h"

int main()
{
  GUI* g=NULL;
  g=init_gui();
  create_main_window(g,"test window",MODE_WINDOW);
  set_main_size(g,600,1000);
  sleep(2);
  show_main(g);
  sleep(2);
  destroy_gui(g);
  return 0;
}
