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
iolib_setdir(8, ENABLE, BBBIO_DIR_OUT);
iolib_setdir(8, STEP, BBBIO_DIR_OUT);
iolib_setdir(8, DIR, BBBIO_DIR_OUT);
iolib_setdir(8, END, BBBIO_DIR_IN);
iolib_setdir(8, STOP, BBBIO_DIR_IN);

const int clk_div = 160;
const int open_dly = 0;
const int sample_dly = 1;
unsigned int buffer_AIN_0[BUFFER_SIZE] ={0};

BBBIO_ADCTSC_module_ctrl(BBBIO_ADC_WORK_MODE_TIMER_INT, clk_div);
BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN0, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly,BBBIO_ADC_STEP_AVG_1, buffer_AIN_0, BUFFER_SIZE);

BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN0);

pin_low(8,ENABLE);
double ini=0;
pin_high(8,DIR);
while(is_high(8,END)){
        for(i=0;i<10;i++){
                pin_high(8,STEP);
                usleep(3);
                pin_low(8,STEP);
                usleep(200);
	}
}

BBBIO_ADCTSC_work(SAMPLE_SIZE);
for(j = 0 ; j < SAMPLE_SIZE ; j++) {
        sample = buffer_AIN_0[j];
        ini=ini+((float)sample / (float)4095.0) * (float)1.8;
}

while(is_low(8,STOP)){
	old_val=vout;
	vout=0.0;
	BBBIO_ADCTSC_work(SAMPLE_SIZE);
	for(j = 0 ; j < SAMPLE_SIZE ; j++) {
		sample = buffer_AIN_0[j];
		vout=vout+((float)sample / (float)4095.0) * (float)1.8;
	}
	vout=vout/SAMPLE_SIZE;
	vout=((vout/97.1)+0.00000019)/0.0004754;
	diff=(double)setpoint-((double)vout-ini);
	if(sqrt(diff*diff)<0.001){
		pin_high(8,ENABLE);
		printf("Final Pounds: %lf\tSteps: %lu\n",vout-ini,steps);
		usleep(10000);
	}
else{
pin_low(8,ENABLE);
	if(diff>0){
		pin_low(8,DIR);
		for(i=0;i<10;i++){
			pin_high(8,STEP);
			usleep(3);
			pin_low(8,STEP);
			usleep(200);
			steps++;
		}
	}
	else{
                pin_high(8,DIR);
                for(i=0;i<1;i++){
                        pin_high(8,STEP);
                        usleep(3);
                        pin_low(8,STEP);
                        usleep(200);
                        steps--;
                }

	}
}
//	usleep(1000);
}

iolib_free();
return 0;
}
