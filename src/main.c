#include <stdio.h>
#include <unistd.h>
#include "graphics.h"

void my_exit(void* data)
{
  shutdown_gui(data);
}

int main()
{
  GUI* g=NULL;
  WIDGET* testLab=NULL;

  g=init_gui();

  testLab=create_label("Label",10,10);;
  
  create_main_window(g,"Testing Window");
  set_main_size(g,500,500);
  add_to_main(g,testLab);

  show_main(g);

  while(gui_running(g)){
    usleep(100);
  }

  destroy_gui(g);
  return 0;
}
