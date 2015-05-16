#ifndef GUI_H_
#define GUI_H_

#include "graphics.h"
#include "logger.h"
#include "data_structures.h"
#include "control.h"

void add_pics(GUI* g,LOGGER* log);
void module(GUI* g, LOGGER* log,int num,struct module_t* mod);
void extern_force(GUI* g,LOGGER* log,struct extern_force_t* dev);
void extern_grip(GUI* g, LOGGER* log,struct extern_grip_t* dev);
void start_all(GUI* g, LOGGER* log,struct master_start_stop_t* data);

#endif
