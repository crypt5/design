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
  WIDGET* testBut=NULL;
  WIDGET* testRadio=NULL;
  WIDGET* testBox=NULL;
  WIDGET* simpleBorder=NULL;
  WIDGET* simpleLabel=NULL;
  WIDGET* hardBorder=NULL;
  WIDGET* text=NULL;
  WIDGET* but=NULL;

  g=init_gui();

  testLab=create_label("Label",10,10);
  testBut=create_button("Exit",430,465,my_exit,g);
  testRadio=create_radio_button("Radio Button",10,30);
  testBox=create_checkbox("Checkbox",10,60);
  simpleLabel=create_label("Simple Border Element",10,100);
  simpleBorder=create_border(5,95,25,200,3);
  hardBorder=create_titled_border("Titled Border",5,140,25,200,3);
  text=create_textbox(10,180,20);
  but=create_button("Button",10,220,NULL,NULL);
  
  create_main_window(g,"Testing Window");
  set_main_size(g,500,500);
  add_to_main(g,testLab);
  add_to_main(g,testBut);
  add_to_main(g,testRadio);
  add_to_main(g,testBox);
  add_to_main(g,simpleLabel);
  add_to_main(g,simpleBorder);
  add_to_main(g,hardBorder);
  add_to_main(g,text);
  add_to_main(g,but);

  show_main(g);

  while(gui_running(g)){
    usleep(100);
  }

  destroy_gui(g);
  return 0;
}
