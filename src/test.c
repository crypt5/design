#include <unistd.h>
#include "graphics.h"

int main()
{
  GUI* g=NULL;
  WIDGET* label=NULL;
  int i=0,vis;

  g=init_gui();
  create_main_window(g,"Widget Testing");
  set_main_size(g,500,500);

  label=create_label("Hello World",10,10);
  set_label_visible(label,1);
  add_to_main(g,label);


  show_main(g);
  while(gui_running(g)){
    if(i==2){
      vis=get_label_enable(label);
      if(vis==1)
	set_label_enable(label,0);
      else
	set_label_enable(label,1);
      update_widget(g,label);
      i=0;
    }
    sleep(1);
    i++;
  }
  destroy_gui(g);
}
