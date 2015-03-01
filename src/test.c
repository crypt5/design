#include <unistd.h>
#include "graphics.h"

void button_callback(GUI* g,WIDGET* w,void* data)
{
  WIDGET* text=data;
  set_titled_border_visible(text,0);
  update_widget(g,text);
}

int main()
{
  GUI* g=NULL;
  WIDGET* button=NULL;
  WIDGET* border=NULL;

  g=init_gui();
  create_main_window(g,"Widget Testing");
  set_main_size(g,500,500);

  button=create_button("Click me!",10,45);
  border=create_titled_border("Hello World",5,5,200,200);
  set_titled_border_thickness(border,3);
  set_titled_border_color(border,0x00FF00FF);
  set_titled_border_text_color(border,0x0000FF00);

  add_to_main(g,button);
  add_to_main(g,border);


  set_button_callback(button,button_callback,border);

  show_main(g);
  while(gui_running(g)){
    usleep(100);
  }
  destroy_gui(g);
}
