#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "graphics.h"

char* printer(void* data)
{
  return (char*)data;
}

int main()
{
  char** data=NULL;
  int i;
  GUI* g=NULL;
  WIDGET* combo=NULL;
  
  data=malloc(sizeof(char*)*5);
  for(i=0;i<5;i++)
    data[i]=malloc(10);
  strcpy(data[0],"Hello");
  strcpy(data[1],"From");
  strcpy(data[2],"Beyond");
  strcpy(data[3],"The");
  strcpy(data[4],"Grave");
  
  g=init_gui();
  create_main_window(g,"Widget Testing");
  set_main_size(g,500,500);

  combo=create_combobox(20,10,10,printer);
  add_item_to_combobox(combo,data[0]);
  add_item_to_combobox(combo,data[1]);
  add_item_to_combobox(combo,data[2]);
  add_item_to_combobox(combo,data[3]);
  add_item_to_combobox(combo,data[4]);

  add_to_main(g,combo);

  show_main(g);
  while(gui_running(g)){
    usleep(100);
  }
  destroy_gui(g);
  
  for(i=0;i<5;i++)
    free(data[i]);
  free(data);
  
}
