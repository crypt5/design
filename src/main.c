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
  WIDGET* testRadio=NULL;
  WIDGET* testCheck=NULL;
  WIDGET* testBorder=NULL;
  WIDGET* titleBorder=NULL;

  g=init_gui();

  testLab=create_label("Hello World!",10,10);
  testBut=create_button("This is a Button!",10,40);
  //set_button_callback(testBut,my_exit,g);
  testRadio=create_radio_button("Click me!",10,70);
  testCheck=create_checkbox("Click Me!",10,100);
  testBorder=create_simple_border(5,7,120,200);
  titleBorder=create_titled_border("Titled Border",5,140,40,200);
  
  create_main_window(g,"Testing Window");
  set_main_size(g,500,500);
  add_to_main(g,testLab);
  add_to_main(g,testBut);
  add_to_main(g,testRadio);
  add_to_main(g,testCheck);
  add_to_main(g,testBorder);
  add_to_main(g,titleBorder);

  show_main(g);

  while(gui_running(g)){
    usleep(100);
  }

  destroy_gui(g);
  return 0;
}
