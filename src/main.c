#include <stdlib.h>
#include <stdio.h>
#include "gui.h"
#include "logger.h"
#include "config.h"
#include "graphics.h"
#include "data_logger.h"
#include "data_structures.h"
#include "control.h"
#include "BBBiolib.h"

#include <unistd.h>

int main()
{
  LOGGER* log=NULL;
  DATA_LOGGER* data_out=NULL;
  GUI* ui=NULL;
  char buf[1024];
  CONFIG* config=NULL;
  struct gui_extern_force_widgets_t* ex_force=NULL;
  struct gui_extern_grip_widgets_t* ex_grip=NULL;
  struct gui_start_stop_status_t* start_stop=NULL;
  struct module_t* mod1=NULL;
  struct module_t* mod2=NULL;

  iolib_init();

  log=logger_init("logs/Program_Output.log");
  logger_log(log,"[Logger] Logging started");
  config=config_init();
  logger_log(log,"[Config] Object Created");
  logger_log(log,"[Config] Loading file: config/main.cfg");
  config_load_file(config,"config/main.cfg");
  logger_log(log,"[Config] File Read, parsed, and loaded");

  mod1=setup_actuator_module("8.12","8.13","8.14","8.15","8.16",0);
  mod2=setup_actuator_module("9.11","9.12","9.13","9.14","9.15",1);

  logger_log(log,"[GUI] Creating GUI Object");
  ui=init_gui();
  create_main_window(ui,"Hand Study Device");
  set_main_icon(ui,"res/icons/main.xpm");
  set_main_size(ui,500,1010);
  logger_log(log,"[GUI] Window built");
  logger_log(log,"[GUI] Building GUI");
  add_pics(ui,log);
  module(ui,log,0,mod1);
  module(ui,log,1,mod2);
  ex_force=extern_force(ui,log);
  ex_grip=extern_grip(ui,log);
  start_stop=start_all(ui,log);



  logger_log(log,"[Data Logger] Starting");
  data_out=data_logger_init("output.csv");

  logger_log(log,"[GUI] Displaying window");
  show_main(ui);
  while(gui_running(ui)){
    usleep(10000);
    sprintf(buf,"%0.0f",get_current_actuator_vaule(mod1));
    set_textfield_text(mod1->interface->current_output,buf);
    update_widget(ui,mod1->interface->current_output);

    sprintf(buf,"%0.0f",get_current_actuator_vaule(mod2));
    set_textfield_text(mod2->interface->current_output,buf);
    update_widget(ui,mod2->interface->current_output);
  }

  free(ex_force);
  free(ex_grip);
  free(start_stop);
  destroy_actuator_module(mod1);
  destroy_actuator_module(mod2);

  logger_log(log,"[Data Logger] Shutting Down");
  data_logger_shutdown(data_out);
  logger_log(log,"[GUI] Shutting down GUI");
  destroy_gui(ui);
  logger_log(log,"[Config] Closing and Freeing object");
  config_destroy(config);
  logger_log(log,"[Logger] Shutting down");
  logger_shutdown(log);
}
