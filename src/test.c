#include <unistd.h>
#include <stdio.h>
#include "graphics.h"

void click(GUI* g, WIDGET* welf, void* data)
{
  int enable;
  WIDGET* box=(WIDGET*)data;
  enable=get_textfield_enable(box);

  if(enable==1)
    set_textfield_enable(box,0);
  else
    set_textfield_enable(box,1);

  update_widget(g,box);
}

int main()
{
  GUI* g=NULL;
  WIDGET* check=NULL;
  WIDGET* button=NULL;

  g=init_gui();
  create_main_window(g,"Testing Window");
  set_main_size(g,400,400);

  check=create_textfield(10,10,40);

  button=create_button("enable",10,50);
  set_button_callback(button,click,check);

  add_to_main(g,check);
  add_to_main(g,button);

  show_main(g);
  while(gui_running(g)){
    usleep(500000);
  }

  destroy_gui(g);
}
