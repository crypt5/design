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
#include "defs.h"

#include <unistd.h>

int main()
{
  LOGGER* log=NULL;
  DATA_LOGGER* data_out=NULL;
  GUI* ui=NULL;
  CONFIG* config=NULL;
  struct extern_force_t* ex_force=NULL;
  struct gui_extern_grip_widgets_t* ex_grip=NULL;
  struct gui_start_stop_status_t* start_stop=NULL;
  struct module_t* mod1=NULL;
  struct module_t* mod2=NULL;

  log=logger_init("logs/Program_Output.log");
  logger_log(log,"[Logger] Logging started");
  config=config_init();
  logger_log(log,"[Config] Object Created");
  logger_log(log,"[Config] Loading file: config/main.cfg");
  config_load_file(config,"config/main.cfg");
  logger_log(log,"[Config] File Read, parsed, and loaded");

#ifdef MICRO
  logger_log(log,"[IOLIB] Initilizing and Setting up ADC");
  iolib_init();
  const int clk_div = 1;
  BBBIO_ADCTSC_module_ctrl(BBBIO_ADC_WORK_MODE_BUSY_POLLING, clk_div);
  logger_log(log,"[IOLIB] Ready");
#endif
  /* AIN 4,5,6 are Avalible */
  mod1=setup_actuator_module("8.07","8.08","8.09","8.10","8.11",4);
  mod2=setup_actuator_module("8.12","8.13","8.14","8.15","8.16",5);
  ex_force=setup_extern_force_device(6);

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
  extern_force(ui,log,ex_force);
  ex_grip=extern_grip(ui,log);
  start_stop=start_all(ui,log);



  logger_log(log,"[Data Logger] Starting");
  data_out=data_logger_init("output.csv");

  logger_log(log,"[GUI] Displaying window");
  show_main(ui);
  while(gui_running(ui)){ 
    usleep(250000);
  }

  free(ex_grip);
  free(start_stop);
  destroy_extern_force_device(ex_force);
  destroy_actuator_module(mod1);
  destroy_actuator_module(mod2);

#ifdef MICRO
  iolib_free();
#endif

  logger_log(log,"[Data Logger] Shutting Down");
  data_logger_shutdown(data_out);
  logger_log(log,"[GUI] Shutting down GUI");
  destroy_gui(ui);
  logger_log(log,"[Config] Closing and Freeing object");
  config_destroy(config);
  logger_log(log,"[Logger] Shutting down");
  logger_shutdown(log);
}
