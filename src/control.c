#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "control.h"

void* run_motor(void* data)
{
  struct module_t* mod=data;
  pthread_mutex_lock(&mod->lock);
  int runner=mod->run;
  pthread_mutex_unlock(&mod->lock);

  while(runner){
    //TODO Motor Control Routuine
    printf("Controling motor!\n");
    sleep(1);
    pthread_mutex_lock(&mod->lock);
    runner=mod->run;
    mod->return_value++;
    pthread_mutex_unlock(&mod->lock);
  }

  return NULL;
}


int get_header(char* str)
{
  return atoi(&str[0]);
}

int get_pin(char* str)
{
  return atoi(str+2);
}

struct module_t* setup_actuator_module(char* enable, char* dir, char* step,char* far_sensor,char* near_sensor,int AIN)
{
  int mutex_create;
  struct module_t* re=NULL;
  re=malloc(sizeof(struct module_t));
  if(re==NULL){
    printf("Module setup data malloc failed\n");
    exit(-1);
  }
  re->interface=NULL;
  mutex_create=pthread_mutex_init(&re->lock,NULL);
  if(mutex_create!=0){
    printf("Mutex Creation for module failed\n");
    exit(-1);
  }
  re->run=1;
  re->mode=-1;
  re->set_value=-1;
  re->return_value=0;
  re->dir_header=get_header(dir);
  re->dir_pin=get_pin(dir);
  re->step_header=get_header(step);
  re->step_pin=get_pin(step);
  re->enable_header=get_header(enable);
  re->enable_pin=get_pin(enable);
  re->far_sensor_header=get_header(far_sensor);
  re->far_sensor_pin=get_pin(far_sensor);
  re->near_sensor_header=get_header(near_sensor);
  re->near_sensor_pin=get_pin(near_sensor);
  re->AIN_pin=AIN;

  return re;
}

void start_actuator(struct module_t* mod,int mode,double desired_value)
{
  int create;
  mod->mode=mode;
  mod->set_value=desired_value;
  mod->run=1;
  create=pthread_create(&mod->id,NULL,run_motor,mod);
  if(create!=0){
    printf("Motor Driving Thread creation failed!\n");
    exit(-1);
  }
}

void stop_actuator(struct module_t* mod)
{
  int mode;
  pthread_mutex_lock(&mod->lock);
  mod->run=0;
  mode=mod->mode;
  pthread_mutex_unlock(&mod->lock);
  if(mode>=0)
    pthread_join(mod->id,NULL);
}

void set_actuator_desired_value(struct module_t* mod,double value)
{
  pthread_mutex_lock(&mod->lock);
  mod->set_value=value;
  pthread_mutex_unlock(&mod->lock);
}

double get_current_actuator_vaule(struct module_t* mod)
{
  double re;
  pthread_mutex_lock(&mod->lock);
  re=mod->return_value;
  pthread_mutex_unlock(&mod->lock);
  return re;
}


void destroy_actuator_module(struct module_t* mod)
{
  int running;
  pthread_mutex_lock(&mod->lock);
  running=mod->run;
  pthread_mutex_unlock(&mod->lock);
  
  if(running==1)
    stop_actuator(mod);

  pthread_mutex_destroy(&mod->lock);
  free(mod->interface);
  free(mod);
}
