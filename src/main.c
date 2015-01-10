#include <stdio.h>
#include <unistd.h>
#include "graphics.h"

int main()
{
  GUI* g=NULL;
  g=init_gui();
  create_main_window(g);
  sleep(2);
  destroy_gui(g);
  return 0;
}
