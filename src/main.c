#include <unistd.h>
#include "logger.h"
#include "config.h"
#include "gui.h"

int main()
{
  GUI* gui=NULL;
  LOGGER* log=NULL;
  CONFIG* con=NULL;

  log=logger_init("logs/test.log");
  logger_log(log,"Creating Config Reader");
  con=config_init();
  logger_log(log,"Config reader ready");
  logger_log(log,"No config files to load");
  logger_log(log,"Building GUI");
  gui=build_gui();
  logger_log(log,"GUI built");
  logger_log(log,"Entering Main loop");

  while(is_open(gui)){
    usleep(100);
    //This is where data collection can take place
  }

  logger_log(log,"Exiting main Loop");
  logger_log(log,"Shutting down GUI");
  clean_up_gui(gui);
  logger_log(log,"Cleaning up the config reader");
  config_destroy(con);
  logger_log(log,"Shutting down Logger");
  logger_shutdown(log);
}
