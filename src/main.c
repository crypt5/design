#include "gui.h"
#include "logger.h"
#include "config.h"
#include "graphics.h"
#include "data_logger.h"

#include <unistd.h>

int main()
{
  LOGGER* log=NULL;
  DATA_LOGGER* data_out=NULL;
  GUI* ui=NULL;
  CONFIG* config=NULL;

  log=logger_init("logs/Program_Output.log");
  logger_log(log,"[Logger] Logging started");
  config=config_init();
  logger_log(log,"[Config] Object Created");
  logger_log(log,"[Config] Loading file: config/main.cfg");
  config_load_file(config,"config/main.cfg");
  logger_log(log,"[Config] File Read, parsed, and loaded");

  logger_log(log,"[GUI] Creating GUI Object");
  ui=init_gui();
  logger_log(log,"[GUI] Building GUI");
  build_ui(ui,log);


  logger_log(log,"[Data Logger] Starting");
  data_out=data_logger_init("output.csv");

  logger_log(log,"[GUI] Displaying window");
  show_main(ui);
  while(gui_running(ui)){
    usleep(10000);
  }


  logger_log(log,"[Data Logger] Shutting Down");
  data_logger_shutdown(data_out);
  logger_log(log,"[GUI] Shutting down GUI");
  destroy_gui(ui);
  logger_log(log,"[Config] Closing and Freeing object");
  config_destroy(config);
  logger_log(log,"[Logger] Shutting down");
  logger_shutdown(log);
}
