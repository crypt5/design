#include <stdio.h>
#include <unistd.h>
#include "graphics.h"

void my_print(void* data)
{
  printf("Hello World\n");
}

int main()
{
  GUI* g=NULL;
  WIDGET* testLab=NULL;
  WIDGET* testBut=NULL;

  testLab=create_label("Hello World!",10,10);

  testBut=create_button("Test Button",100,100,my_print,NULL);

  g=init_gui();
  create_main_window(g,"Testing Window");
  set_main_size(g,500,500);
  add_to_main(g,testLab);

  show_main(g);

  while(gui_running(g))
    sleep(1);

  destroy_gui(g);
  return 0;
}
