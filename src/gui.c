#include "graphics.h"

void button_group(GUI* g,WIDGET* one, void* data)
{
  WIDGET* two=NULL;
  int my_val;
  two=data;
  my_val=get_radio_button_check(one);
  if(my_val==1)
    set_radio_button_check(two,0);
  else
    set_radio_button_check(two,1);
  update_widget(g,one);
  update_widget(g,two);
}

void my_exit(GUI* g,WIDGET* w, void* d)
{
  shutdown_gui(g);
}

GUI* build_gui()
{
  GUI* g=NULL;
  WIDGET* tech=NULL;
  WIDGET* tech_border=NULL;
  WIDGET* unm=NULL;
  WIDGET* unm_border=NULL;
  WIDGET* force_border=NULL;
  WIDGET* displacement_border=NULL;
  WIDGET* mode_border=NULL;
  WIDGET* desired_force_label=NULL;
  WIDGET* measured_force_label=NULL;
  WIDGET* desired_displacement_label=NULL;
  WIDGET* measured_displacement_label=NULL;
  WIDGET* desired_force=NULL;
  WIDGET* measured_force=NULL;
  WIDGET* desired_displacement=NULL;
  WIDGET* measured_displacement=NULL;
  WIDGET* force_mode=NULL;
  WIDGET* displacement_mode=NULL;
  WIDGET* start=NULL;
  WIDGET* stop=NULL;
  WIDGET* exit=NULL;

  g=init_gui();
  create_main_window(g,"UNM Hand Study Device");
  set_main_size(g,500,500);

  //Pictures
  tech=create_label("NMT Logo",10,10);
  tech_border=create_simple_border(5,5,100,240);
  unm=create_label("UNM Logo",260,10);
  unm_border=create_simple_border(250,5,100,245);

  //Force
  force_border=create_titled_border("Force",5,120,100,490);
  desired_force_label=create_label("Desired Force:",87,130);
  measured_force_label=create_label("Measured Force:",80,170);
  desired_force=create_textbox(230,130,15);
  measured_force=create_textbox(230,170,15);

  //Displacement
  displacement_border=create_titled_border("Displacement",5,240,100,490);
  desired_displacement_label=create_label("Desired Displacement:",65,250);
  measured_displacement_label=create_label("Measured Displacement:",60,290);
  desired_displacement=create_textbox(270,250,15);
  measured_displacement=create_textbox(270,290,15);

  //Buttons
  start=create_button("Start",30,400);
  stop=create_button("Stop",400,400);
  exit=create_button("Exit",435,470);

  //Mode selector
  mode_border=create_titled_border("Mode",160,360,120,180);
  force_mode=create_radio_button("Force",175,380);
  displacement_mode=create_radio_button("Displacement",175,420);

  add_to_main(g,tech);
  add_to_main(g,tech_border);
  add_to_main(g,unm);
  add_to_main(g,unm_border);
  add_to_main(g,force_border);
  add_to_main(g,desired_force_label);
  add_to_main(g,measured_force_label);
  add_to_main(g,desired_force);
  add_to_main(g,measured_force);
  add_to_main(g,displacement_border);
  add_to_main(g,desired_displacement_label);
  add_to_main(g,measured_displacement_label);
  add_to_main(g,desired_displacement);
  add_to_main(g,measured_displacement);
  add_to_main(g,start);
  add_to_main(g,stop);
  add_to_main(g,mode_border);
  add_to_main(g,force_mode);
  add_to_main(g,displacement_mode);
  add_to_main(g,exit);

  set_button_callback(exit,my_exit,NULL);
  set_radio_button_callback(force_mode,button_group,displacement_mode);
  set_radio_button_callback(displacement_mode,button_group,force_mode);

  show_main(g);
  return g;
}

void clean_up_gui(GUI* g)
{
  destroy_gui(g);
}

int is_open(GUI* g)
{
  return gui_running(g);
}
