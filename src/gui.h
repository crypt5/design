#ifndef GUI_H_
#define GUI_H_

#include "graphics.h"
#include "logger.h"
#include "data_structures.h"
#include "control.h"

void add_pics(GUI* g,LOGGER* log);
void module(GUI* g, LOGGER* log,int num,struct module_t* mod);
void extern_force(GUI* g,LOGGER* log,struct extern_force_t* dev);
struct gui_extern_grip_widgets_t* extern_grip(GUI* g, LOGGER* log);
struct gui_start_stop_status_t* start_all(GUI* g, LOGGER* log);

#endif
