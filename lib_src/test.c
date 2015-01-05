#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "headers/logger.h"

void my_free(void* data)
{
  free(data);
}

int main()
{
  LOGGER* log=NULL;

  log=logger_init("test.log");
  logger_log(log,"[INFO] Test Message 1");
  logger_log(log,"[DEBUG] Test Message 2");
  logger_log(log,"Test Message 3");
  logger_log(log,"[FINE] Test Message 4");
  logger_log(log,"Test Message 5");
  logger_log(log,"[FINNE] Final Message");
  logger_shutdown(log);
  return 0;
}
