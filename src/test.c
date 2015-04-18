#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "BBBiolib.h"

#define ENABLE 12
#define STEP 14
#define DIR 16
#define END 15
#define STOP 13

#define BUFFER_SIZE 100
#define SAMPLE_SIZE 10

int main(int argc, char** argv)
{
if(argc<2){
printf("USAGE [PROG] # where # is number of pounds Desired\n");
exit(-1);
}
int i,j,sample;
double vout=0,old_val,diff;
int setpoint=atoi(argv[1]);
unsigned long steps=0;

iolib_init();
BBBIO_ADCTSC_Init();
iolib_setdir(8, ENABLE, BBBIO_DIR_OUT); // General pin IO settings
iolib_setdir(8, STEP, BBBIO_DIR_OUT); 	// ""
iolib_setdir(8, DIR, BBBIO_DIR_OUT); 	// ""
iolib_setdir(8, END, BBBIO_DIR_IN); 	// ""
iolib_setdir(8, STOP, BBBIO_DIR_IN); 	// ""

const int clk_div = 160;
const int open_dly = 0;
const int sample_dly = 1;
unsigned int buffer_AIN_0[BUFFER_SIZE] ={0};

BBBIO_ADCTSC_module_ctrl(BBBIO_ADC_WORK_MODE_TIMER_INT, clk_div);
BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN0, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_1, buffer_AIN_0, BUFFER_SIZE);

BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN0); // Turns on ADC reading in load cell

pin_low(8,ENABLE); 			// Turns on motor (active low)
double ini=0; 				// Initializes the inital force reading to 0
pin_high(8,DIR); 			// Sets direction outward from motor
while(is_high(8,END)){ 			// Until we've reached calibration photointerrupter...
        for(i=0;i<10;i++){ 		// In ten 1/8 step increments...
                pin_high(8,STEP); 	// Generate actuation signal
                usleep(3); 		// ""
                pin_low(8,STEP); 	// ""
                usleep(200); 		// ""
	}
}

BBBIO_ADCTSC_work(SAMPLE_SIZE); 					       // gets initial force reading 
for(j = 0 ; j < SAMPLE_SIZE ; j++) {
        sample = buffer_AIN_0[j];
        ini=ini+((float)sample / (float)4095.0) * (float)1.8;                  // Converts initial force reading to volts
} 
ini=ini/SAMPLE_SIZE;//Thanks Chris
while(is_low(8,STOP)){ 								// when calibration photointerrupter reached
	old_val=vout; 								// set past ADC Vout as current one before...
	vout=0.0; 		 						// initializing ADC Vout reading to 0 V for next reading
	BBBIO_ADCTSC_work(SAMPLE_SIZE); 					// fetch 10 readings
	for(j = 0 ; j < SAMPLE_SIZE ; j++) { 					// goes to sum them to get average value 
		sample = buffer_AIN_0[j]; 					// 
		vout=vout+((float)sample / (float)4095.0) * (float)1.8; 	// convert ADC readings to voltage & sum them
	}
	vout=vout/SAMPLE_SIZE; 							// Divide by # of readings to get an averaged reading
	vout=((vout/97.1)+0.00000019)/0.0004754; 				// Put it through the Load cell conversion factor to turn volts to pounds
	diff=(double)setpoint-((double)vout-ini); 				// Get the error; Why subtract vout by init?
	
	if(sqrt(diff*diff)<0.001){ 				 		// If |error| is less than 0.001, then...
		pin_high(8,ENABLE); 						// Turn off motor
		printf("Final Pounds: %lf\tSteps: %lu\n",vout-ini,steps); 	// Print status to screen
		usleep(10000); 							// go to sleep
	}
	
	else{ 									// else we control the motor to fix error 
		pin_low(8,ENABLE); 						// Turn on the Motor (active low)
		if(diff>0){   							// If positive error, pull line towards motor
			pin_low(8,DIR);
			for(i=0;i<10;i++){ 					// Travel in 10 increment micro steps
				pin_high(8,STEP);
				usleep(3);
				pin_low(8,STEP);
				usleep(200);
				steps++; 					// Increment Step counter (shouldn't it be by 10?)
			}
		}
		else{
                	pin_high(8,DIR); 		 			// else if negative error, push line outwards
                	for(i=0;i<1;i++){ 					// travel in 1 incement micro steps
                        	pin_high(8,STEP);
                        	usleep(3);
                        	pin_low(8,STEP);
                        	usleep(200);
                        	steps--; 					// Decrement Step counter
        		}

		}
	}
//	usleep(1000);
}

iolib_free();
return 0;
}
