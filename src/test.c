#include <unistd.h>
#include <stdio.h>
#include "iolib.h"

int main()
{
  iolib_init();
  //Loop enable
  iolib_setdir(8,11,DIR_IN);

  // Drection Pin
  iolib_setdir(8,12,DIR_OUT);

  // Toggle Pin
  iolib_setdir(8,13,DIR_OUT);

  while(is_high(8,11)){
    int i;

    pin_high(8,12);
    sleep(1);

    for(i=0;i<1000;i++){
      pin_low(8,13);
      pin_high(8,13);
      usleep(400);
    }

    pin_low(8,12);
    sleep(1);

    for(i=0;i<1000;i++){
      pin_low(8,13);
      pin_high(8,13);
      usleep(400);
    }
  }
pin_low(8,12);
pin_low(8,13);


  iolib_free();
}
