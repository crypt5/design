#include <unistd.h>
#include "graphics.h"


int main()
{
  GUI* g=NULL;
  WIDGET* pic=NULL;

  g=init_gui();
  create_main_window(g,"Widget Testing");
  set_main_size(g,500,500);

  pic=create_picture(g,"res/test.xpm",10,10);

  add_to_main(g,pic);

  show_main(g);
  while(gui_running(g)){
    usleep(100);
  }
  destroy_gui(g);
}
