#include <stdio.h>
#include "graphics.h"
#include "logger.h"

void build_ui(GUI* g,LOGGER* log)
{
  create_main_window(g,"Hand Study Device");
  set_main_icon(g,"res/icons/main.xpm");
  set_main_size(g,500,500);
  logger_log(log,"[GUI] Window built");

}
