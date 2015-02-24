#include <unistd.h>
#include "graphics.h"

int main()
{
  GUI* g=NULL;
  WIDGET* label=NULL;
  WIDGET* border=NULL;
  WIDGET* button=NULL;

  g=init_gui();
  create_main_window(g,"Widget Testing");
  set_main_size(g,500,500);

  label=create_label("Hello World",10,10);
  set_label_visible(label,1);

  border=create_simple_border(8,8,30,120);
  set_simple_border_thickness(border,1);
  set_simple_border_color(border,0x0000FF00);

  button=create_button("Click me!",10,45);
  set_button_text_color(button,0);
  set_button_enable(button,1);

  add_to_main(g,label);
  add_to_main(g,border);
  add_to_main(g,button);


  show_main(g);
  while(gui_running(g)){
    usleep(100);
  }
  destroy_gui(g);
}
