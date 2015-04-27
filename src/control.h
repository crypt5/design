#ifndef CONTROL_H_
#define CONTROL_H_

#include <pthread.h>
#include "graphics.h"
#include "data_structures.h"

struct module_t{
  struct gui_module_widgets_t* interface;
  pthread_mutex_t lock;
  pthread_t id;
  int run;
  int mode;
  double set_value;
  double return_value;
  int dir_header;
  int dir_pin;
  int enable_header;
  int enable_pin;
  int step_header;
  int step_pin;
  int far_sensor_header;
  int far_sensor_pin;
  int near_sensor_header;
  int near_sensor_pin;
  int AIN_pin;
};

//Actuator functions
struct module_t* setup_actuator_module(char* enable, char* dir, char* step,char* far_sensor,char* near_sensor,int AIN);
void start_actuator(struct module_t* mod,int mode,double desired_value);
void stop_actuator(struct module_t* mod);
void set_actuator_desired_value(struct module_t* mod,double value);
double get_current_actuator_vaule(struct module_t* mod);
void destroy_actuator_module(struct module_t* mod);

#endif
