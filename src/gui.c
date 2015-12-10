#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "graphics.h"
#include "logger.h"
#include "gui.h"
#include "data_structures.h"
#include "callbacks.h"
#include "control.h"

void add_pics(GUI* g,LOGGER* log)
{
  logger_log(log,"[GUI] Adding pictures");
  WIDGET* nmt=NULL;
  WIDGET* unm=NULL;

  nmt=create_picture(g,"res/robotics_lab.xpm",10,10);
  unm=create_picture(g,"res/unm_orth.xpm",415,10);

  add_to_main(g,nmt);
  add_to_main(g,unm);
  logger_log(log,"[GUI] Pictures Added");
}

void module(GUI* g, LOGGER* log,int num,struct module_t* mod)
{

  char buf[1024];
  sprintf(buf,"[GUI] Creating Module %d Layout",num+1);
  logger_log(log,buf);

  struct gui_module_widgets_t* data=NULL;
  data=malloc(sizeof(struct gui_module_widgets_t));
  if(data==NULL){
    printf("Module widget data holding malloc failed!\n");
    exit(-1);
  }
  sprintf(buf,"Module %d",num+1);
  data->border=create_titled_border(buf,20+(330*num),130,280,320);
  set_titled_border_thickness(data->border,2);
  data->enable_device=create_checkbox("Enable Module",30+(330*num),140);
  set_checkbox_callback(data->enable_device,module_enable_callback,data);

  data->device_status=create_label("Not Active",200+(330*num),140);
  data->seperator1=create_seperator(30+(330*num),170,280);
  data->mode_label=create_label("Operation Mode:",30+(330*num),175);
  set_label_enable(data->mode_label,0);
  data->force_mode=create_radio_button("Force Mode",40+(330*num),200);
  set_radio_button_check(data->force_mode,1);
  set_radio_button_enable(data->force_mode,0);
  set_radio_button_callback(data->force_mode,force_displacement_button_group_callback,data);
  data->displacement_mode=create_radio_button("Displacement Mode",40+(330*num),225);
  set_radio_button_enable(data->displacement_mode,0);
  set_radio_button_callback(data->displacement_mode,force_displacement_button_group_callback,data);
  data->seperator2=create_seperator(30+(330*num),255,280);
  data->desired_section_label=create_label("Desired",30+(330*num),260);
  set_label_enable(data->desired_section_label,0);
  data->desired_label=create_label("Force:",30+(330*num),280);
  set_label_enable(data->desired_label,0);
  data->desired_enter=create_textfield(155+(330*num),280,8);
  set_textfield_enable(data->desired_enter,0);
  data->desired_unit=create_label("Newtons",235+(330*num),280);
  set_label_enable(data->desired_unit,0);
  data->start_device=create_button("Start Module",30+(330*num),315);
  set_button_enable(data->start_device,0);
  set_button_callback(data->start_device,start_module_callback,mod);
  data->stop_device=create_button("Stop Module",190+(330*num),315);
  set_button_enable(data->stop_device,0);
  set_button_callback(data->stop_device,stop_module_callback,mod);
  data->seperator3=create_seperator(30+(330*num),350,280);
  data->current_section_label=create_label("Current",30+(330*num),355);  
  set_label_enable(data->current_section_label,0);
  data->current_output_label=create_label("Force:",30+(330*num),375);
  set_label_enable(data->current_output_label,0);
  data->current_output=create_textfield(155+(330*num),375,8);
  set_textfield_enable(data->current_output,0);
  set_textfield_editable(data->current_output,0);
  data->current_output_unit=create_label("Newtons",235+(330*num),375);
  set_label_enable(data->current_output_unit,0);

  add_to_main(g,data->border);
  add_to_main(g,data->enable_device);
  add_to_main(g,data->device_status);
  add_to_main(g,data->seperator1);
  add_to_main(g,data->mode_label);
  add_to_main(g,data->force_mode);
  add_to_main(g,data->displacement_mode);
  add_to_main(g,data->seperator2);
  add_to_main(g,data->desired_section_label);
  add_to_main(g,data->desired_label);
  add_to_main(g,data->desired_enter);
  add_to_main(g,data->desired_unit);
  add_to_main(g,data->start_device);
  add_to_main(g,data->stop_device);
  add_to_main(g,data->seperator3);
  add_to_main(g,data->current_section_label);
  add_to_main(g,data->current_output_label);
  add_to_main(g,data->current_output);
  add_to_main(g,data->current_output_unit);

  sprintf(buf,"[GUI] Module %d Created",num+1);
  logger_log(log,buf);
  mod->interface=data;
  mod->g=g;
}


void extern_force(GUI* g,LOGGER* log,struct extern_force_t* dev)
{
  logger_log(log,"[GUI] Creating External Force Measurment Device");
  struct gui_extern_force_widgets_t* data=NULL;
  data=malloc(sizeof(struct gui_extern_force_widgets_t));
  if(data==NULL){
    printf("Extern force widget data holding malloc failed!\n");
    exit(-1);
  }

  data->border=create_titled_border("External Force Device",700,130,110,220);
  set_titled_border_thickness(data->border,2);

  data->enable_device=create_checkbox("Enable Device",720,140);
  set_checkbox_callback(data->enable_device,external_force_device_enable_callback,dev);

  data->seperator=create_seperator(710,170,180);

  data->output_label=create_label("Measured Force:",720,175);
  set_label_enable(data->output_label,0);

  data->output_display=create_textfield(740,200,9);
  set_textfield_editable(data->output_display,0);
  set_textfield_enable(data->output_display,0);

  data->output_unit=create_label("Newtons",840,200);
  set_label_enable(data->output_unit,0);

  add_to_main(g,data->border);
  add_to_main(g,data->enable_device);
  add_to_main(g,data->seperator);
  add_to_main(g,data->output_label);
  add_to_main(g,data->output_display);
  add_to_main(g,data->output_unit);
  logger_log(log,"[GUI] External Force Measurment Device Created");
  dev->interface=data;
  dev->g=g;
}


void extern_grip(GUI* g, LOGGER* log,struct extern_grip_t* dev)
{
  logger_log(log,"[GUI] Creating External Grip Measurment Device");
  struct gui_extern_grip_widgets_t* data=NULL;
  data=malloc(sizeof(struct gui_extern_grip_widgets_t));
  if(data==NULL){
    printf("Extern Grip widget data holding malloc failed!\n");
    exit(-1);
  }
  data->border=create_titled_border("External Grip Device",700,260,150,250);
  set_titled_border_thickness(data->border,2);

  data->enable_device=create_checkbox("Enable Device",720,270);
  set_checkbox_callback(data->enable_device,external_grip_device_enable_callback,dev);

  data->seperator=create_seperator(710,300,210);

  data->displacement_label=create_label("Current Displacement:",720,305);
  set_label_enable(data->displacement_label,0);

  data->displacement_output=create_textfield(740,330,9);
  set_textfield_editable(data->displacement_output,0);
  set_textfield_enable(data->displacement_output,0);

  data->displacement_unit=create_label("millimeter",840,330);
  set_label_enable(data->displacement_unit,0);

  data->force_label=create_label("Measured Force:",720,355);
  set_label_enable(data->force_label,0);

  data->force_output=create_textfield(740,380,9);
  set_textfield_editable(data->force_output,0);
  set_textfield_enable(data->force_output,0);

  data->force_unit=create_label("Newtons",840,380);
  set_label_enable(data->force_unit,0);

  add_to_main(g,data->border);
  add_to_main(g,data->enable_device);
  add_to_main(g,data->seperator);
  add_to_main(g,data->displacement_label);
  add_to_main(g,data->displacement_output);
  add_to_main(g,data->displacement_unit);
  add_to_main(g,data->force_label);
  add_to_main(g,data->force_output);
  add_to_main(g,data->force_unit);

  dev->interface=data;
  dev->g=g;

  logger_log(log,"[GUI] External Grip Measurment Device Created");
}



void start_all(GUI* g, LOGGER* log,struct master_start_stop_t* d)
{
  logger_log(log,"[GUI] Creating File Output and Start-Stop Buttons");
  struct gui_start_stop_status_t* data=NULL;
  data=malloc(sizeof(struct gui_start_stop_status_t));
  if(data==NULL){
    printf("Start/Stop test widget data holding malloc failed!\n");
    exit(-1);
  }
  data->start_test=create_button("Start All Modules and Record Data",20,420);
  set_button_callback(data->start_test,master_start_callback,d);
  data->stop_test=create_button("Stop Modules and Write Data",720,420);
  set_button_enable(data->stop_test,0);
  set_button_callback(data->stop_test,master_stop_callback,d);

  data->status_label=create_label("Status:",420,420);
  data->status=create_label("Not Running",490,420);
  data->file_display_label=create_label("Output File:",20,455);
  data->file_display=create_textfield(140,455,93);
  set_textfield_editable(data->file_display,0);


  add_to_main(g,data->start_test);
  add_to_main(g,data->stop_test);
  add_to_main(g,data->status_label);
  add_to_main(g,data->status);
  add_to_main(g,data->file_display_label);
  add_to_main(g,data->file_display);
  logger_log(log,"[GUI] File Output and Start-Stop Buttons created");
  d->interface=data;
}
