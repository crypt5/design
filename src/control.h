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
  int enable;
  GUI* g;
  pthread_mutex_t lock;
  pthread_t id;
  int run;
  double return_val;
  int AIN;
};

struct extern_grip_t{
  struct gui_extern_grip_widgets_t* interface;
  int enable;
  GUI* g;
  pthread_mutex_t lock;
  pthread_t id;
  int run;
  unsigned long steps;
  double distance;
  double force;
  double spring_k;
  int chan_a_header;
  int chan_a_pin;
  int chan_b_header;
  int chan_b_pin;
};

struct master_start_stop_t{
  struct gui_start_stop_status_t* interface;
  int log_data;
  int mod1;
  int mod2;
  int force;
  int grip;
  struct module_t* one;
  struct module_t* two;
  struct extern_force_t* f;
  struct extern_grip_t* g;
};

//Actuator functions
struct module_t* setup_actuator_module(char* enable, char* dir, char* step,char* far_sensor,char* near_sensor,int AIN);
void start_actuator(struct module_t* mod,int mode,double desired_value);
void stop_actuator(struct module_t* mod);
void set_actuator_desired_force(struct module_t* mod,double value);
int get_module_enabled(struct module_t* mod);
double get_current_actuator_force(struct module_t* mod);
void set_actuator_desired_displacement(struct module_t* mod,double value);
double get_current_actuator_displacement(struct module_t* mod);
void destroy_actuator_module(struct module_t* mod);

//External Force functions
struct extern_force_t* setup_extern_force_device(int AIN);
void start_extern_force_device(struct extern_force_t* device);
double get_force_value(struct extern_force_t* device);
int get_extern_force_enable(struct extern_force_t* device);
void stop_extern_force_device(struct extern_force_t* device);
void destroy_extern_force_device(struct extern_force_t* device);

//External Grip Functions
struct extern_grip_t* setup_extern_grip_device(char* chan_A, char* chan_B,double K);
void start_extern_grip_device(struct extern_grip_t* device);
double get_extern_grip_force(struct extern_grip_t* device);
double get_extern_grip_displacement(struct extern_grip_t* device);
int get_extern_grip_enable(struct extern_grip_t* device);
void stop_extern_grip_device(struct extern_grip_t* device);
void destroy_extern_grip_device(struct extern_grip_t* device);

//Master start stop functions
struct master_start_stop_t* create_start_stop_data();
void destroy_start_stop_data(struct master_start_stop_t* dev);
#endif
