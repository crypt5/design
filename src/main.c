#include <stdio.h>
#include <unistd.h>
#include "graphics.h"

void my_exit(WIDGET* self,void* data)
{
  shutdown_gui(data);
}

int main()
{
  GUI* g=NULL;
  WIDGET* testLab=NULL;
  WIDGET* testBut=NULL;

  g=init_gui();

  testLab=create_label("Hello World!",10,10);
  testBut=create_button("This is a Button!",10,40);
  set_button_callback(testBut,my_exit,g);
  
  create_main_window(g,"Testing Window");
  set_main_size(g,500,500);
  add_to_main(g,testLab);
  add_to_main(g,testBut);

  show_main(g);

  while(gui_running(g)){
    usleep(100);
  }

  destroy_gui(g);
  return 0;
}
