#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include "graphics.h"
#include "control.h"
#include "BBBiolib.h"
#include "defs.h"

#define BUFFER_SIZE 50
#define SAMPLE_SIZE 50

#define PEAK_LEN 5
#define WAVE_LEN 100

#define STEPS_PER_MM 26000

/**************** MOTOR Section **************************/

void* run_motor(void* data)
{
  struct module_t* mod=data;
  pthread_mutex_lock(&mod->lock);
  int runner=mod->run;
  int mode=mod->mode;
  double disp=mod->set_displacement;
  pthread_mutex_unlock(&mod->lock);


#ifdef MICRO
  int i;
  double steps=0;
  pin_low(mod->enable_header,mod->enable_pin);
  pin_high(mod->dir_header,mod->dir_pin);
  while(is_high(mod->far_sensor_header,mod->far_sensor_pin)){
    for(i=0;i<10;i++){
      pin_high(mod->step_header,mod->step_pin);
      usleep(PEAK_LEN);
      pin_low(mod->step_header,mod->step_pin);
      usleep(WAVE_LEN);
      }
  }
  //Main Control Loop for force
  int j;
  double voltage=0,old_voltage,differance,read_force,setpoint;
  while(runner && mode==MODE_FORCE){
    old_voltage=voltage;
    voltage=0;
    BBBIO_ADCTSC_work(SAMPLE_SIZE); 
   	for(j = 0 ; j < SAMPLE_SIZE ; j++) {
		  voltage=voltage+((double)mod->ain_buffer[j] / (double)4095.0) * (double)1.8;
	  }
    voltage=voltage/(double)SAMPLE_SIZE;
	  read_force=((voltage/97.1)+0.00000019)/0.0004754;
    differance=(double)setpoint-((double)read_force); 
     
   	if(sqrt(differance*differance)<0.001){
		  pin_high(mod->enable_header,mod->enable_pin);
		  usleep(100);
	  }
    else {
      pin_low(mod->enable_header,mod->enable_pin); //moving in
		  if(differance>0){
        if(is_high(mod->near_sensor_header,mod->near_sensor_pin)){
			    pin_low(mod->dir_header,mod->dir_pin);
          for(i=0;i<10;i++){
            pin_high(mod->step_header,mod->step_pin);
            usleep(PEAK_LEN);
            pin_low(mod->step_header,mod->step_pin);
            usleep(WAVE_LEN);
            steps++;
          }
        }
        else {
          pin_high(mod->enable_header,mod->enable_pin);
        }
      }
	    else{
         if(is_high(mod->far_sensor_header,mod->far_sensor_pin)){
           pin_high(mod->dir_header,mod->dir_pin); //moving out
	         for(i=0;i<10;i++){
             pin_high(mod->step_header,mod->step_pin);
             usleep(PEAK_LEN);
             pin_low(mod->step_header,mod->step_pin);
             usleep(WAVE_LEN);
             steps--;
 	         }
         }
        else {
          pin_high(mod->enable_header,mod->enable_pin);
        }
      }
    }
     
    pthread_mutex_lock(&mod->lock);
    runner=mod->run;
    setpoint=mod->set_force;
    mod->return_force=read_force;
    mod->return_displacement=steps;
    pthread_mutex_unlock(&mod->lock);
    usleep(1);
  }
  
  //Main control loop for Displacement
  steps=0;
  while(runner && mode==MODE_DISPLACEMENT){
    old_voltage=voltage;
    voltage=0;
    BBBIO_ADCTSC_work(SAMPLE_SIZE); 
   	for(j = 0 ; j < SAMPLE_SIZE ; j++) {
		  voltage=voltage+((double)mod->ain_buffer[j] / (double)4095.0) * (double)1.8;
	  }
    voltage=voltage/(double)SAMPLE_SIZE;
	  read_force=((voltage/97.1)+0.00000019)/0.0004754;
    
    if(abs(steps-(disp*(double)STEPS_PER_MM))<0.01){
      pin_high(mod->enable_header,mod->enable_pin);
		  usleep(100);
    }
    else {
      pin_low(mod->enable_header,mod->enable_pin);
      if(steps<(disp*STEPS_PER_MM)){
          if(is_high(mod->near_sensor_header,mod->near_sensor_pin)){
            pin_low(mod->dir_header,mod->dir_pin);
            for(i=0;i<10;i++){ //Moving in
              pin_high(mod->step_header,mod->step_pin);
              usleep(PEAK_LEN);
              pin_low(mod->step_header,mod->step_pin);
              usleep(WAVE_LEN);
              steps++;
            }
          }
          else{
            pin_high(mod->enable_header,mod->enable_pin);
          }
      }
      else {
        printf("Steps: %lf\nDISP: %lf\n",steps,disp*STEPS_PER_MM);
      }
    
    }
     
   	
    pthread_mutex_lock(&mod->lock);
    runner=mod->run;
    setpoint=mod->set_force;
    mod->return_force=read_force;
    mod->return_displacement=steps;
    pthread_mutex_unlock(&mod->lock);
    usleep(1);
  }
  
  pin_low(mod->enable_header,mod->enable_pin);
  pin_high(mod->dir_header,mod->dir_pin);
  while(is_high(mod->far_sensor_header,mod->far_sensor_pin)){
    for(i=0;i<10;i++){
      pin_high(mod->step_header,mod->step_pin);
      usleep(PEAK_LEN);
      pin_low(mod->step_header,mod->step_pin); 
      usleep(WAVE_LEN);
    }
  }
  pin_high(mod->enable_header,mod->enable_pin); 
  
#else //Regular computer
  while(runner){
    usleep(500000);
    if(mod->mode==1)
      printf("Controlling Motor Force.\tSet Value=%lf\n",mod->set_force);
    else
      printf("Controlling Motor Displacement.\tSet Value=%lf\n",mod->set_displacement);
          pthread_mutex_lock(&mod->lock);
    runner=mod->run;
    pthread_mutex_unlock(&mod->lock);
  }
#endif
 
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
  re->set_force=-1;
  re->return_force=0;
  re->set_displacement=-1;
  re->return_displacement=0;
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

#ifdef MICRO
  
  iolib_setdir(re->step_header, re->step_pin, BBBIO_DIR_OUT);//Step
  iolib_setdir(re->dir_header, re->dir_pin, BBBIO_DIR_OUT);//Direction
  iolib_setdir(re->enable_header, re->enable_pin, BBBIO_DIR_OUT);//Enable
  iolib_setdir(re->far_sensor_header, re->far_sensor_pin, BBBIO_DIR_IN);
  iolib_setdir(re->near_sensor_header, re->near_sensor_pin, BBBIO_DIR_IN);
  pin_high(re->enable_header,re->enable_pin);
  
  const int open_dly = 0;
  const int sample_dly = 1;
  switch(AIN){
  case 0:
    BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN0, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_1, re->ain_buffer, BUFFER_SIZE);
    BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN0);
  break;
  case 1:
    BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN1, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_1, re->ain_buffer, BUFFER_SIZE);
    BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN1);
  break;
  case 2:
    BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN2, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_1, re->ain_buffer, BUFFER_SIZE);
    BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN2);
  break;
  case 3:
    BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN3, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_1, re->ain_buffer, BUFFER_SIZE);
    BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN3);
  break;
  case 4:
    BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN4, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_1, re->ain_buffer, BUFFER_SIZE);
    BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN4);
  break;
  case 5:
    BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN5, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_1, re->ain_buffer, BUFFER_SIZE);
    BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN5);
  break;
  case 6:
    BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN6, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_1, re->ain_buffer, BUFFER_SIZE);
    BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN6);
  break;
}

#endif

  return re;
}

void start_actuator(struct module_t* mod,int mode,double desired_value)
{
  int create;
  mod->mode=mode;
  if(mode==MODE_FORCE)//Force Mode
    mod->set_force=desired_value;
  else if(mode==MODE_DISPLACEMENT)//Displacement mode
    mod->set_displacement=desired_value;
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

void set_actuator_desired_force(struct module_t* mod,double value)
{
  pthread_mutex_lock(&mod->lock);
  mod->set_force=value;
  pthread_mutex_unlock(&mod->lock);
}

int get_module_enabled(struct module_t* mod)
{
  return get_checkbox_checked(mod->interface->enable_device);
}

double get_current_actuator_force(struct module_t* mod)
{
  double re;
  pthread_mutex_lock(&mod->lock);
  re=mod->return_force;
  pthread_mutex_unlock(&mod->lock);
  return re;
}

void set_actuator_desired_displacement(struct module_t* mod,double value)
{
  pthread_mutex_lock(&mod->lock);
  mod->set_displacement=value;
  pthread_mutex_unlock(&mod->lock);
}

double get_current_actuator_displacement(struct module_t* mod)
{
  double re;
  pthread_mutex_lock(&mod->lock);
  re=mod->return_displacement;
  re=re/STEPS_PER_MM;
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

/************************* EXTERNAL FORCE DEVICE ********************/

void* run_ex_force(void *data)
{
  int runner;
  int count=0;
  struct extern_force_t* device=data;
  GUI* g=device->g;
  WIDGET* output=device->interface->output_display;  

  pthread_mutex_lock(&device->lock);
  runner=device->run;
  pthread_mutex_unlock(&device->lock);

#ifdef MICRO
  int j,sample;
  double ini=0;
  const int open_dly = 0;
  const int sample_dly = 1;
  unsigned int buffer_AIN[BUFFER_SIZE] ={0};
  switch(device->AIN){
  case 0:
    BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN0, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_16, buffer_AIN, BUFFER_SIZE);
    BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN0);
  break;
  case 1:
    BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN1, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_16, buffer_AIN, BUFFER_SIZE);
    BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN1);
  break;
  case 2:
    BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN2, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_16, buffer_AIN, BUFFER_SIZE);
    BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN2);
  break;
  case 3:
    BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN3, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_16, buffer_AIN, BUFFER_SIZE);
    BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN3);
  break;
  case 4:
    BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN4, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_16, buffer_AIN, BUFFER_SIZE);
    BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN4);
  break;
  case 5:
    BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN5, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_16, buffer_AIN, BUFFER_SIZE);
    BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN5);
  break;
  case 6:
    BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN6, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_16, buffer_AIN, BUFFER_SIZE);
    BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN6);
  break;
  default:
    printf("No Matching AIN pin for %d\n",device->AIN);
    exit(-1);
  }
  while(runner){
    BBBIO_ADCTSC_work(SAMPLE_SIZE);
    for(j = 0 ; j < SAMPLE_SIZE ; j++) {
        sample = buffer_AIN[j];
        ini=ini+(((float)sample / (float)4095.0) * (float)1.8);
    }
    
    ini=ini/SAMPLE_SIZE;
    ini=((ini/97.1)+0.00000019)/0.0004754;
    
    pthread_mutex_lock(&device->lock);
    runner=device->run;
    device->return_val=ini;
    pthread_mutex_unlock(&device->lock);

    if(count>99){
      char buf[50];
      sprintf(buf,"%0.4lf",ini);
      set_textfield_text(output,buf);
      update_widget(g,output);
      count=0;
    }
    count++;
    ini=0;
  }
#else
  while(runner){
    printf("Reading Force Value\n");
    usleep(500000);
    pthread_mutex_lock(&device->lock);
    runner=device->run;
    pthread_mutex_unlock(&device->lock);
    count++;
    device->return_val++;
    if(count>9){
      char buf[50];
      sprintf(buf,"%0.4lf",device->return_val);
      set_textfield_text(output,buf);
      update_widget(g,output);
      count=-1;
    }
  }
#endif

  return NULL;
}

struct extern_force_t* setup_extern_force_device(int AIN)
{
  int mutex_create;
  struct extern_force_t* re=NULL;
  re=malloc(sizeof(struct extern_force_t));
  if(re==NULL){
    printf("Extern Force setup data malloc failed\n");
    exit(-1);
  }
  re->interface=NULL;
  mutex_create=pthread_mutex_init(&re->lock,NULL);
  if(mutex_create!=0){
    printf("Mutex Creation for module failed\n");
    exit(-1);
  }
  re->run=0;
  re->return_val=0;
  re->AIN=AIN;

  return re;
}

void start_extern_force_device(struct extern_force_t* device)
{
  int create;
  device->run=1;
  device->enable=1;
  create=pthread_create(&device->id,NULL,run_ex_force,device);
  if(create!=0){
    printf("Ex Force reading Thread creation failed!\n");
    exit(-1);
  }
}

double get_force_value(struct extern_force_t* device)
{
  double re;
  pthread_mutex_lock(&device->lock);
  re=device->return_val;
  pthread_mutex_unlock(&device->lock);
  return re;
}

int get_extern_force_enable(struct extern_force_t* device)
{
  int re;
  pthread_mutex_lock(&device->lock);
  re=device->enable;
  pthread_mutex_unlock(&device->lock);
  return re;
}

void stop_extern_force_device(struct extern_force_t* device)
{
  pthread_mutex_lock(&device->lock);
  device->run=0;
  device->enable=0;
  pthread_mutex_unlock(&device->lock);
  pthread_join(device->id,NULL);
}

void destroy_extern_force_device(struct extern_force_t* device)
{
  int running;
  pthread_mutex_lock(&device->lock);
  running=device->run;
  pthread_mutex_unlock(&device->lock);

  if(running==1)
    stop_extern_force_device(device);

  pthread_mutex_destroy(&device->lock);
  free(device->interface);
  free(device);
}


/************ External Grip Device Section ****************/
void* run_ex_grip(void *data)
{
  int runner;
  struct extern_grip_t* device=data;
  GUI* g=device->g;

  pthread_mutex_lock(&device->lock);
  runner=device->run;
  pthread_mutex_unlock(&device->lock);

#ifdef MICRO
  //TODO Micro controller code
#else
  while(runner){
    printf("Reading Grip Device\n");
    usleep(500000);
    pthread_mutex_lock(&device->lock);
    runner=device->run;
    pthread_mutex_unlock(&device->lock);
  }
#endif

  return NULL;
}

struct extern_grip_t* setup_extern_grip_device(char* chan_A, char* chan_B,double K)
{
  int mutex_create;
  struct extern_grip_t* re=NULL;
  re=malloc(sizeof(struct extern_grip_t));
  if(re==NULL){
    printf("Extern Grip setup data malloc failed\n");
    exit(-1);
  }
  re->interface=NULL;
  mutex_create=pthread_mutex_init(&re->lock,NULL);
  if(mutex_create!=0){
    printf("Mutex Creation for extern grip failed\n");
    exit(-1);
  }
  re->run=0;
  re->steps=0;
  re->distance=0;
  re->force=0;
  re->spring_k=K;
  re->chan_a_header=get_header(chan_A);
  re->chan_a_pin=get_pin(chan_A);
  re->chan_b_header=get_header(chan_B);
  re->chan_b_pin=get_pin(chan_B);
  return re;
}

void start_extern_grip_device(struct extern_grip_t* device)
{
  int create;
  device->run=1;
  device->enable=1;
  create=pthread_create(&device->id,NULL,run_ex_grip,device);
  if(create!=0){
    printf("Ex grip reading Thread creation failed!\n");
    exit(-1);
  }
}

double get_extern_grip_force(struct extern_grip_t* device)
{
  double re;
  pthread_mutex_lock(&device->lock);
  re=device->force;
  pthread_mutex_unlock(&device->lock);
  return re;
}

double get_extern_grip_displacement(struct extern_grip_t* device)
{
  double re;
  pthread_mutex_lock(&device->lock);
  re=device->distance;
  pthread_mutex_unlock(&device->lock);
  return re;
}

int get_extern_grip_enable(struct extern_grip_t* device)
{
  int re;
  pthread_mutex_lock(&device->lock);
  re=device->enable;
  pthread_mutex_unlock(&device->lock);
  return re;
}

void stop_extern_grip_device(struct extern_grip_t* device)
{
  pthread_mutex_lock(&device->lock);
  device->run=0;
  device->enable=0;
  pthread_mutex_unlock(&device->lock);
  pthread_join(device->id,NULL);
}

void destroy_extern_grip_device(struct extern_grip_t* device)
{
  int running;
  pthread_mutex_lock(&device->lock);
  running=device->run;
  pthread_mutex_unlock(&device->lock);

  if(running==1)
    stop_extern_grip_device(device);

  pthread_mutex_destroy(&device->lock);
  free(device->interface);
  free(device);
}

/*************** Master Start Stop Functions **************/
struct master_start_stop_t* create_start_stop_data()
{
  struct master_start_stop_t* re=NULL;
  re=malloc(sizeof(struct master_start_stop_t));
  if(re==NULL){
    printf("Start Stop data holder malloc failed\n");
    exit(-1);
  }
  re->log_data=0;
  re->mod1=0;
  re->mod2=0;
  re->force=0;
  re->grip=0;
  re->one=NULL;
  re->two=NULL;
  re->f=NULL;
  re->g=NULL;

  return re;
}

void destroy_start_stop_data(struct master_start_stop_t* dev)
{
  free(dev->interface);
  free(dev);
}