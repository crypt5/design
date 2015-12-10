#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "graphics.h"
#include "data_structures.h"
#include "callbacks.h"
#include "control.h"

void external_force_device_enable_callback(GUI* g, WIDGET* w,void* data)
{
  int enable=get_checkbox_checked(w);
  struct extern_force_t* device=data;
  struct gui_extern_force_widgets_t* gui=device->interface;
  set_label_enable(gui->output_label,enable);
  set_textfield_enable(gui->output_display,enable);
  set_label_enable(gui->output_unit,enable);

  update_widget(g,gui->output_label);
  update_widget(g,gui->output_display);
  update_widget(g,gui->output_unit);

  if(enable){
    start_extern_force_device(device);
  }
  else{
    stop_extern_force_device(device);
    set_textfield_text(gui->output_display,"");
    update_widget(device->g,gui->output_display);
  }
}

void external_grip_device_enable_callback(GUI* g,WIDGET* w,void* data)
{
  int enable=get_checkbox_checked(w);
  struct extern_grip_t* dev=data;
  struct gui_extern_grip_widgets_t* gui=dev->interface;

  set_label_enable(gui->displacement_label,enable);
  set_textfield_enable(gui->displacement_output,enable);
  set_label_enable(gui->displacement_unit,enable);
  set_label_enable(gui->force_label,enable);
  set_textfield_enable(gui->force_output,enable);
  set_label_enable(gui->force_unit,enable);

  if(enable==1){
    start_extern_grip_device(dev);
  }
  else{
    stop_extern_grip_device(dev);
    set_textfield_text(gui->displacement_output,"");
    set_textfield_text(gui->force_output,"");
  }
  
  update_widget(g,gui->displacement_label);
  update_widget(g,gui->displacement_output);
  update_widget(g,gui->displacement_unit);
  update_widget(g,gui->force_label);
  update_widget(g,gui->force_output);
  update_widget(g,gui->force_unit);
}

void module_enable_callback(GUI* g,WIDGET* w,void* data)
{
  int enable=get_checkbox_checked(w);
  struct gui_module_widgets_t* gui=data;

  set_label_enable(gui->mode_label,enable);
  set_label_enable(gui->desired_section_label,enable);
  set_label_enable(gui->desired_unit,enable);
  set_label_enable(gui->desired_label,enable);
  set_label_enable(gui->current_section_label,enable);
  set_label_enable(gui->current_output_label,enable);
  set_label_enable(gui->current_output_unit,enable);
  set_radio_button_enable(gui->force_mode,enable);
  set_radio_button_enable(gui->displacement_mode,enable);
  set_button_enable(gui->start_device,enable);
  set_textfield_enable(gui->current_output,enable);
  set_textfield_enable(gui->desired_enter,enable);

  if(enable==1){
    set_label_text(gui->device_status,"Ready");
    set_label_text_color(gui->device_status,0);
  }
  else{
    set_label_text(gui->device_status,"Not Active");
    set_label_text_color(gui->device_status,0);
  }

  update_widget(g,gui->device_status);
  update_widget(g,gui->mode_label);
  update_widget(g,gui->desired_section_label);
  update_widget(g,gui->desired_unit);
  update_widget(g,gui->desired_label);
  update_widget(g,gui->current_section_label);
  update_widget(g,gui->current_output_label);
  update_widget(g,gui->current_output_unit);
  update_widget(g,gui->force_mode);
  update_widget(g,gui->displacement_mode);
  update_widget(g,gui->start_device);
  update_widget(g,gui->current_output);
  update_widget(g,gui->desired_enter);
}

void force_displacement_button_group_callback(GUI* g,WIDGET* w,void* data)
{
  int force_button=0;
  int checked=get_radio_button_check(w);
  struct gui_module_widgets_t* gui=data;
  if(strcmp(get_radio_button_text(w),"Force Mode")==0)
    force_button=1;

  if(force_button){//Force mode
    if(checked){//in force mode
      set_radio_button_check(gui->displacement_mode,0);
      set_label_text(gui->desired_unit,"Newtons");
      set_label_text(gui->desired_label,"Force");
      set_label_text(gui->current_output_label,"Force");
      set_label_text(gui->current_output_unit,"Newtons");
    }
    else{//in displacement mode
      set_radio_button_check(gui->displacement_mode,1);
      set_label_text(gui->desired_unit,"millimeter");
      set_label_text(gui->desired_label,"Displacement");
      set_label_text(gui->current_output_label,"Displacement");
      set_label_text(gui->current_output_unit,"millimeter");
    }
  }
  else{//Displacement mode
    if(checked){//in displacement mode
      set_radio_button_check(gui->force_mode,0);
      set_label_text(gui->desired_unit,"millimeter");
      set_label_text(gui->desired_label,"Displacement");
      set_label_text(gui->current_output_label,"Displacement");
      set_label_text(gui->current_output_unit,"millimeter");
    }
    else{//in force mode
      set_radio_button_check(gui->force_mode,1);
      set_label_text(gui->desired_unit,"Newtons");
      set_label_text(gui->desired_label,"Force");
      set_label_text(gui->current_output_label,"Force");
      set_label_text(gui->current_output_unit,"Newtons");
    }
  }
  set_textfield_text(gui->current_output,"");
  set_textfield_text(gui->desired_enter,"");
  
  update_widget(g,gui->force_mode);
  update_widget(g,gui->displacement_mode);
  update_widget(g,gui->desired_unit);
  update_widget(g,gui->desired_label);
  update_widget(g,gui->current_output_label);
  update_widget(g,gui->current_output_unit);
  update_widget(g,gui->current_output);
  update_widget(g,gui->desired_enter);
}

void start_module_callback(GUI* g, WIDGET* w,void* data)
{
  struct module_t* all_data=data;
  struct gui_module_widgets_t* gui=all_data->interface;
  char* entry=get_textfield_text(gui->desired_enter);
  char* end;
  int i,mode;
  double val;

  if(strlen(entry)==0)
    return;
  for(i=0;i<strlen(entry);i++){
    if(entry[i]<'0'||entry[i]>'9'){
      if(entry[i]!='.')
	return;
    }
  }

  val=strtod(entry,&end);
  mode=get_radio_button_check(gui->force_mode);
  if(mode==MODE_FORCE){//Force mode
    if(val<0||val>67)
      return;
  }
  else{//Displacement mode
    mode=MODE_DISPLACEMENT;
    if(val<0||val>102)
      return;
  }

  set_button_enable(w,0);
  set_label_text_color(gui->device_status,0x0000FF00);
  set_label_text(gui->device_status,"Running");
  set_textfield_editable(gui->desired_enter,0);
  set_radio_button_enable(gui->force_mode,0);
  set_radio_button_enable(gui->displacement_mode,0);
  set_button_enable(gui->stop_device,1);
  set_checkbox_enable(gui->enable_device,0);

  update_widget(g,w);
  update_widget(g,gui->device_status);
  update_widget(g,gui->desired_enter);
  update_widget(g,gui->force_mode);
  update_widget(g,gui->displacement_mode);
  update_widget(g,gui->stop_device);
  update_widget(g,gui->enable_device);

  start_actuator(data,mode,val);
}

void stop_module_callback(GUI* g,WIDGET* w,void* data)
{
  struct module_t* all_data=data;
  struct gui_module_widgets_t* gui=all_data->interface;

  set_button_enable(w,0);
  set_label_text_color(gui->device_status,0x00FF0000);
  set_label_text(gui->device_status,"Stopped");
  set_textfield_editable(gui->desired_enter,1);
  set_radio_button_enable(gui->force_mode,1);
  set_radio_button_enable(gui->displacement_mode,1);
  set_button_enable(gui->start_device,1);
  set_checkbox_enable(gui->enable_device,1);

  update_widget(g,w);
  update_widget(g,gui->device_status);
  update_widget(g,gui->desired_enter);
  update_widget(g,gui->force_mode);
  update_widget(g,gui->displacement_mode);
  update_widget(g,gui->start_device);
  update_widget(g,gui->enable_device);

  stop_actuator(data);
}

void master_start_callback(GUI* g,WIDGET* w,void *data)
{
  struct master_start_stop_t* test=data;
  test->mod1=get_module_enabled(test->one);
  test->mod2=get_module_enabled(test->two);
  test->force=get_extern_force_enable(test->f);
  test->grip=get_extern_grip_enable(test->g);

  //Start / disable module 1
  if(test->mod1==1){
    start_module_callback(g,test->one->interface->start_device,test->one);
    set_button_enable(test->one->interface->stop_device,0);
  }
  else{
    set_checkbox_enable(test->one->interface->enable_device,0);
    update_widget(g,test->one->interface->enable_device);
  }

  //Start / Disable module 2
  if(test->mod2==1){
    start_module_callback(g,test->two->interface->start_device,test->two);
    set_button_enable(test->two->interface->stop_device,0);
  }
  else{
    set_checkbox_enable(test->two->interface->enable_device,0);
    update_widget(g,test->two->interface->enable_device);
  }

  //Disable extern force toggling
  set_checkbox_enable(test->f->interface->enable_device,0);
  update_widget(g,test->f->interface->enable_device);

  //Disable grip toggling
  set_checkbox_enable(test->g->interface->enable_device,0);
  update_widget(g,test->g->interface->enable_device);

  set_button_enable(w,0);
  set_button_enable(test->interface->stop_test,1);
  set_label_text(test->interface->status,"Running");
  set_label_text_color(test->interface->status,0x0000FF00);

  update_widget(g,test->interface->status);
  update_widget(g,test->interface->stop_test);
  update_widget(g,w);

  test->log_data=1;
}

void master_stop_callback(GUI* g,WIDGET* w,void *data)
{
  struct master_start_stop_t* test=data;

  //Stop / enable module 1
  if(test->mod1==1){
    stop_module_callback(g,test->one->interface->start_device,test->one);
    set_button_enable(test->one->interface->stop_device,1);
  }
  else{
    set_checkbox_enable(test->one->interface->enable_device,1);
    update_widget(g,test->one->interface->enable_device);
  }

  //Stop / enable module 2
  if(test->mod2==1){
    stop_module_callback(g,test->two->interface->start_device,test->two);
    set_button_enable(test->two->interface->stop_device,1);
  }
  else{
    set_checkbox_enable(test->two->interface->enable_device,1);
    update_widget(g,test->two->interface->enable_device);
  }

  //enable extern force toggling
  set_checkbox_enable(test->f->interface->enable_device,1);
  update_widget(g,test->f->interface->enable_device);

  //enable grip toggling
  set_checkbox_enable(test->g->interface->enable_device,1);
  update_widget(g,test->g->interface->enable_device);


  set_button_enable(test->interface->start_test,1);
  set_button_enable(w,0);
  set_label_text(test->interface->status,"Stopped");
  set_label_text_color(test->interface->status,0x00FF0000);

  update_widget(g,test->interface->status);
  update_widget(g,w);
  update_widget(g,test->interface->start_test);

  test->log_data=0;
}
