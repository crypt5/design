#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "iolib.h"
#include "graphics.h"

void button_press(GUI* g, WIDGET* self, void* data)
{
  WIDGET* label=(WIDGET*)data;
  int enable=get_label_visible(label);
  if(enable==0)
    set_label_visible(label,1);
  else
    set_label_visible(label,0);

  update_widget(g,label);
}

int main()
{
  GUI* g=NULL;
  WIDGET* label=NULL;
  WIDGET* button=NULL;

  g=init_gui();
  iolib_init();

  iolib_setdir(8,12,DIR_OUT);
  create_main_window(g,"Widget Testing");
  set_main_size(g,500,500);

  label=create_label("Hello World!",10,10);
  set_label_background(label,0x00FF00FF);
  set_label_text_color(label,0x0000FF00);
  set_label_border(label,0x00000000,1);
  set_label_enable(label,1);

  button=create_button("Disable",10,70);
  set_button_text_color(button,0x00FF00FF);
  set_button_callback(button,button_press,label);

  add_to_main(g,label);
  add_to_main(g,button);

  show_main(g);
int high;
  while(gui_running(g)){
high=is_high(8,12);
if(high==1)
pin_low(8,12);
else
pin_high(8,12);
    usleep(100);
  }
  pin_low(8,12);
  destroy_gui(g);
  iolib_free();
}
