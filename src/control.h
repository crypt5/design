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
  double set_force;
  double return_force;
  double set_displacement;
  double return_displacement;
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

struct extern_force_t{
  struct gui_extern_force_widgets_t* interface;
  GUI* g;
  pthread_mutex_t lock;
  pthread_t id;
  int run;
  double return_val;
  int AIN;
};

//Actuator functions
struct module_t* setup_actuator_module(char* enable, char* dir, char* step,char* far_sensor,char* near_sensor,int AIN);
void start_actuator(struct module_t* mod,int mode,double desired_value);
void stop_actuator(struct module_t* mod);
void set_actuator_desired_force(struct module_t* mod,double value);
double get_current_actuator_force(struct module_t* mod);
void set_actuator_desired_displacement(struct module_t* mod,double value);
double get_current_actuator_displacement(struct module_t* mod);
void destroy_actuator_module(struct module_t* mod);

//External Force functions
struct extern_force_t* setup_extern_force_device(int AIN);
void start_extern_force_device(struct extern_force_t* device);
double get_force_value(struct extern_force_t* device);
void stop_extern_force_device(struct extern_force_t* device);
void destroy_extern_force_device(struct extern_force_t* device);
#endif
