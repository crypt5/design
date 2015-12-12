#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
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
  struct extern_grip_t* ex_grip=NULL;
  struct module_t* mod1=NULL;
  struct module_t* mod2=NULL;
  struct master_start_stop_t* test_data=NULL;
  char first_loop=1;
  char buf[1024];
  struct timeval start_time,current_time;
  int current_run=0;
  double k;

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
  mod1=setup_actuator_module("8.07","8.08","8.09","8.10","8.11",4,1);
  mod2=setup_actuator_module("8.12","8.13","8.14","8.15","8.16",5,2);
  ex_force=setup_extern_force_device(6);
  k=config_get_double(config,"gripKValue")/1000.0;
  ex_grip=setup_extern_grip_device("8.17","8.19",k);

  test_data=create_start_stop_data();
  test_data->one=mod1;
  test_data->two=mod2;
  test_data->f=ex_force;
  test_data->g=ex_grip;

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
  extern_grip(ui,log,ex_grip);
  start_all(ui,log,test_data);


  logger_log(log,"[GUI] Displaying window");
  show_main(ui);
  char* base_name=config_get_string(config, "outputBaseName");

  while(gui_running(ui)){ 
    if(test_data->log_data==1){
      if(first_loop){
        logger_log(log,"[Data Logger] Starting");
        sprintf(buf,"%s-%d.csv",base_name,current_run);
        data_out=data_logger_init(buf);
        set_textfield_text(test_data->interface->file_display,buf);
        update_widget(ui,test_data->interface->file_display);
      
        data_logger_log(data_out, "time (s),\
motor 1 force (N),motor 1 displacement (mm),\
motor 2 force (N),motor 2 displacement (mm),\
external force sensor(N),\
external grip force (N),external grip displacement (mm)");
        first_loop=0;
        gettimeofday(&start_time,NULL);
        current_run++;
      }

      double mot1f,mot1d,mot2f,mot2d,ef,egf,egd,time;
      
      if(test_data->mod1==1){
        mot1f=get_current_actuator_force(test_data->one);
        mot1d=get_current_actuator_displacement(test_data->one);
      } else {
        mot1f=0;
        mot1d=0;
      }
      
      if(test_data->mod2==1){
        mot2f=get_current_actuator_force(test_data->two);
        mot2d=get_current_actuator_displacement(test_data->two);
      } else {
        mot2f=0;
        mot2d=0;
      }
      
      if(test_data->force==1)
        ef=get_force_value(test_data->f);
      else
        ef=0;
        
      if(test_data->grip==1){
        egf=get_extern_grip_force(test_data->g);
        egd=get_extern_grip_displacement(test_data->g);
      } else {
        egf=0;
        egd=0;
      }
      
      gettimeofday(&current_time,NULL);
      time=(current_time.tv_sec-start_time.tv_sec);
      time+=(current_time.tv_usec-start_time.tv_usec)/1000000.0;
      
      sprintf(buf,"%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,",time,mot1f,mot1d,mot2f,mot2d,ef,egf,egd);
      data_logger_log(data_out,buf);
      usleep(1000);
    }
    else{
      if(first_loop==0){
        logger_log(log,"[Data Logger] Shutting Down");
        data_logger_shutdown(data_out);
        first_loop=1;
      }
      usleep(250000);
    }
  }

  destroy_extern_force_device(ex_force);
  destroy_extern_grip_device(ex_grip);
  destroy_actuator_module(mod1);
  destroy_actuator_module(mod2);
  destroy_start_stop_data(test_data);

#ifdef MICRO
  iolib_free();
#endif

  logger_log(log,"[GUI] Shutting down GUI");
  destroy_gui(ui);
  logger_log(log,"[Config] Closing and Freeing object");
  config_destroy(config);
  logger_log(log,"[Logger] Shutting down");
  logger_shutdown(log);
}
