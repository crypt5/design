#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "control.h"
#include "BBBiolib.h"

#define BUFFER_SIZE 100
#define SAMPLE_SIZE 10

void* run_motor(void* data)
{
  int i;
  struct module_t* mod=data;
  pthread_mutex_lock(&mod->lock);
  int runner=mod->run;
  pthread_mutex_unlock(&mod->lock);

  pin_low(mod->enable_header,mod->enable_pin); 			// Turns on motor (active low)
  pin_high(mod->dir_header,mod->dir_pin); 			// Sets direction outward from motor
  while(is_high(mod->far_sensor_header,mod->far_sensor_pin)){ 			// Until we've reached calibration photointerrupter...
    for(i=0;i<10;i++){ 		// In ten 1/8 step increments...
      pin_high(mod->step_header,mod->step_pin); 	// Generate actuation signal
      usleep(5); 		// ""
      pin_low(mod->step_header,mod->step_pin); 	// ""
      usleep(100); 		// ""
    }
  }

  while(runner&&is_low(mod->near_sensor_header,mod->near_sensor_pin)){
      pin_low(mod->dir_header,mod->dir_pin); 
for(i=0;i<10;i++){          // In ten 1/8 step increments...
      pin_high(mod->step_header,mod->step_pin);         // Generate actuation signal
      usleep(5);                // ""
      pin_low(mod->step_header,mod->step_pin);  // ""
      usleep(100);              // ""
    }
	usleep(5000);
    pthread_mutex_lock(&mod->lock);
    runner=mod->run;
    pthread_mutex_unlock(&mod->lock);
  }

  pin_low(mod->enable_header,mod->enable_pin);                  // Turns on motor (active lo$
  pin_high(mod->dir_header,mod->dir_pin);                       // Sets direction outward fr$
  while(is_high(mod->far_sensor_header,mod->far_sensor_pin)){                   // Until we'$
    for(i=0;i<10;i++){          // In ten 1/8 step increments...
      pin_high(mod->step_header,mod->step_pin);         // Generate actuation signal
      usleep(5);                // ""
      pin_low(mod->step_header,mod->step_pin);  // ""
      usleep(100);              // ""
    }
  }

  pin_high(mod->enable_header,mod->enable_pin);  
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

  iolib_setdir(re->step_header, re->step_pin, BBBIO_DIR_OUT);//Step
  iolib_setdir(re->dir_header, re->dir_pin, BBBIO_DIR_OUT);//Direction
  iolib_setdir(re->enable_header, re->enable_pin, BBBIO_DIR_OUT);//Enable
  iolib_setdir(re->far_sensor_header, re->far_sensor_pin, BBBIO_DIR_IN);//Far photo
  iolib_setdir(re->near_sensor_header, re->near_sensor_pin, BBBIO_DIR_IN);//Near Photo

  const int clk_div = 50;
  const int open_dly = 0;
  const int sample_dly = 0;
  unsigned int buffer_AIN_0[BUFFER_SIZE] ={0};

  BBBIO_ADCTSC_module_ctrl(BBBIO_ADC_WORK_MODE_TIMER_INT, clk_div);
  BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN0, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_1, buffer_AIN_0, BUFFER_SIZE);

  BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN0);

  pin_high(re->enable_header,re->enable_pin);

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
