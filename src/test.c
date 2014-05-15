#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "BBBiolib.h"

#define ENABLE 12
#define STEP 14
#define DIR 16

#define BUFFER_SIZE 100
#define SAMPLE_SIZE 10

int main(int argc, char** argv)
{
int i,j,sample;
double vout=0,old_val,diff,lbs;
int setpoint=atoi(argv[1]);


iolib_init();
BBBIO_ADCTSC_Init();
iolib_setdir(8, ENABLE, BBBIO_DIR_OUT);
iolib_setdir(8, STEP, BBBIO_DIR_OUT);
iolib_setdir(8, DIR, BBBIO_DIR_OUT);

const int clk_div = 160;
const int open_dly = 0;
const int sample_dly = 1;
unsigned int buffer_AIN_0[BUFFER_SIZE] ={0};

BBBIO_ADCTSC_module_ctrl(BBBIO_ADC_WORK_MODE_TIMER_INT, clk_div);
BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN0, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_1, buffer_AIN_0, BUFFER_SIZE);

BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN0);

pin_low(8,ENABLE);

while(is_low(8,ENABLE)){
	old_val=vout;
	vout=0.0;
	BBBIO_ADCTSC_work(SAMPLE_SIZE);
	for(j = 0 ; j < SAMPLE_SIZE ; j++) {
		sample = buffer_AIN_0[j];
		vout=vout+((float)sample / (float)4095.0) * (float)1.8;
	}
	vout=vout/SAMPLE_SIZE;
	vout=((vout/97.1)+0.00000019)/0.0004754;
	diff=(double)setpoint-((double)vout-4.0);
	if(diff<(double)0.1||diff<0){
		pin_high(8,ENABLE);
	}
	printf("Diff: %lf\tVout: %lf\n",diff,vout-4);
	pin_low(8,DIR);
	for(i=0;i<100;i++){
		pin_high(8,STEP);
		usleep(3);
		pin_low(8,STEP);
		usleep(300);
	}

//	usleep(1000);
}

iolib_free();
return 0;
}
