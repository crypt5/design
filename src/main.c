#include <stdio.h>
#include <unistd.h>
#include "graphics.h"

int main()
{
  GUI* g=NULL;
  WIDGET* testLab=NULL;

  testLab=create_label("Hello World!",10,10);

  g=init_gui();
  create_main_window(g,"Testing Window");
  set_main_size(g,600,1000);
  add_to_main(g,testLab);

  show_main(g);

  while(gui_running(g))
    sleep(1);

  destroy_gui(g);
  return 0;
}
