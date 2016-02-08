#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "control.h"
#include "BBBiolib.h"
#include "config.h"
#include "defs.h"

/**
 * This function gets the 1000 sample average
 * for the supplied module
 * @param module_t* mod, A pointer to the module to use
 * @return an 1000 sample average value
 */
double get_average(struct module_t* mod)
{
	int num=1000;
	int i;
	double sum=0;
	for(i=0;i<num;i++){
		sum=sum+get_current_actuator_force(mod);
		usleep(1000);
	}
	return sum/(double)num;
}

/**
 * Main function for the calibration program.
 */
int main()
{
	int mod_num,AIN;
	struct module_t* mod;
  double weight[5];
  double voltage[5];
	CONFIG* c=config_init();
	config_load_file(c,"config/main.cfg");
	char *enable,
		 *dir,
		 *step,
		 *far_sensor,
		 *near_sensor,
		 buf[50];

#ifdef MICRO
	iolib_init();
	const int clk_div = 1;
	BBBIO_ADCTSC_module_ctrl(BBBIO_ADC_WORK_MODE_BUSY_POLLING, clk_div);
#endif

	//Menu Start
	printf("Welcome to the calibration Program\nfor the Hand Study Device\n\n");

	//Choose a Module to Calibrate
	printf("Please Enter the number of the\nmodule you want to calibrate(1,2): ");
	scanf("%d",&mod_num);
	if((mod_num!=1)&&(mod_num!=2)){
		printf("Please enter Either 1 or 2\nExiting. . . \n");
		exit(-1);
	}

	//Get Config Values for Module
	printf("\nReading Configuration for Module %d . . . \n",mod_num);

	//Get Enable Pin From File
	sprintf(buf,"module%dEnablePin",mod_num);
	enable=config_get_string(c,buf);
	printf("\tEnable Pin Set to: \t\t%s\n",enable);

	//Get direction Pin From File
	sprintf(buf,"module%dDirectionPin",mod_num);
	dir=config_get_string(c,buf);
	printf("\tDirection Pin Set to: \t\t%s\n",dir);

	//Get Step Pin From File
	sprintf(buf,"module%dStepPin",mod_num);
	step=config_get_string(c,buf);
	printf("\tStep Pin Set to: \t\t%s\n",step);

	//Get Far Sensor Pin From File
	sprintf(buf,"module%dFarSensorPin",mod_num);
	far_sensor=config_get_string(c,buf);
	printf("\tFar Sensor Pin Set to: \t\t%s\n",far_sensor);

	//Get Near Sensor Pin From File
	sprintf(buf,"module%dNearSensorPin",mod_num);
	near_sensor=config_get_string(c,buf);
	printf("\tNear Sensor Pin Set to: \t%s\n",near_sensor);

	//Get AIN Pin Number From File
	sprintf(buf,"module%dAIN",mod_num);
	AIN=config_get_int(c,buf);
	printf("\tAIN Pin Number Set to: \t\t%d\n",AIN);

	printf("Done Loading\n");

	//Start Testing Phase
	printf("\nStarting Module\n");
	mod=setup_actuator_module(enable,dir,step,far_sensor,near_sensor,AIN,0,1);
  start_actuator(mod,MODE_DISPLACEMENT,20);
  printf("Please Wait until Motor Stops Moving . . . \n");
  printf("Press Y when stopped. (Y): ");
  scanf("%s",&buf);
  
  int i=0;
  for(i=0;i<5;i++){
    printf("\tEnter mass number %d (in grams): ",i+1);
    scanf("%lf",&weight[i]);
    weight[i]=(weight[i]/1000.0)*9.81;
    printf("\tPress Y when ready to take reading: ");
    scanf("%s",&buf);
    voltage[i]=get_average(mod);
    printf("\tRead Voltage: %lf\n\n",voltage[i]);
  }
  
  
  printf("Calculating Linerar Regression\n");
  double meanx,meany;
  meany=(weight[0]+weight[1]+weight[2]+weight[3]+weight[4])/5;
  meanx=(voltage[0]+voltage[1]+voltage[2]+voltage[3]+voltage[4])/5;
  double top=0,bot=0;
  for(i=0;i<5;i++){
    top=top+((voltage[i]-meanx)*(weight[i]-meany));
    bot=bot+((voltage[i]-meanx)*(voltage[i]-meanx));
  }
  printf("\nModule %d slope value: %lf\n",mod_num,(top/bot));
  printf("Module %d offset value: %lf\n\n",mod_num,(meany-((top/bot)*meanx)));
  
  // Clean up memory
  stop_actuator(mod);
#ifdef MICRO
	iolib_free();
#endif
	destroy_actuator_module(mod);
	config_destroy(c);
	return 0;
}

