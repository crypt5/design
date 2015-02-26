#include <unistd.h>
#include "graphics.h"

void button_callback(GUI* g,WIDGET* w,void* data)
{
  WIDGET* radio=data;
  int enabled=get_radio_button_enable(radio);
  if(enabled==1)
    enabled=0;
  else
    enabled=1;
  set_radio_button_enable(radio,enabled);
  update_widget(g,radio);
}

int main()
{
  GUI* g=NULL;
  WIDGET* label=NULL;
  WIDGET* border=NULL;
  WIDGET* button=NULL;
  WIDGET* radio=NULL;

  g=init_gui();
  create_main_window(g,"Widget Testing");
  set_main_size(g,500,500);

  label=create_label("Hello World",10,10);
  border=create_simple_border(8,8,30,120);
  button=create_button("Click me!",10,45);

  radio=create_radio_button("Check me!",10,70);
  set_radio_button_text_color(radio,0x00FF00FF);
  set_radio_button_check_color(radio,0x00FF00FF);
  set_radio_button_check(radio,1);

  add_to_main(g,label);
  add_to_main(g,border);
  add_to_main(g,button);
  add_to_main(g,radio);


  set_button_callback(button,button_callback,radio);

  show_main(g);
  while(gui_running(g)){
    usleep(100);
  }
  destroy_gui(g);
}
