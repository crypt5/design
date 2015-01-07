#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "headers/config.h"


int main()
{
  CONFIG* c=NULL;
  int test=-1;

  c=config_init();
  config_load_file(c,"test.cfg");
  test=config_get_boolean(c,"testBool");
  if(test!=VALUE_NOT_FOUND)
    printf("Value of testNum:%d\n",test);
  else
    printf("Value not found\n");
  config_destroy(c);
  return 0;
}
