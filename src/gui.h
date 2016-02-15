#ifndef GUI_H_
#define GUI_H_

#include "Graphics/graphics.h"
#include "kenutil/logger.h"
#include "data_structures.h"
#include "control.h"

/**
 * This function adds the pictures to the GUI
 * @param GUI* g, The pointer to the GUI object
 * @param LOGGER* log, pointer to the logger
 */
void add_pics(GUI* g,LOGGER* log);

/**
 * This function sets up the GUI for the module
 * @param GUI* g, The pointer for the GUI
 * @param LOGGER* log, Pointer for the logging service
 * @param num, The number for the module, used in layout
 * @param module_t* mod, a pointer for the module's info
 */
void module(GUI* g, LOGGER* log,int num,struct module_t* mod);

/**
 * This function sets up the GUI for the external force device
 * @param GUI* g, The pointer for the GUI
 * @param LOGGER* log, Pointer for the logging service
 * @param extern_force_t* dev, a pointer to the device
 */
void extern_force(GUI* g,LOGGER* log,struct extern_force_t* dev);

/**
 * This function sets up the GUI for the external grip device
 * @param GUI* g, The pointer for the GUI
 * @param LOGGER* log, Pointer for the logging service
 * @param extern_grip_t* dev, a pointer to the device
 */
void extern_grip(GUI* g, LOGGER* log,struct extern_grip_t* dev);

/**
 * This function sets up the GUI for the master start and stop buttons
 * @param GUI* g, The pointer for the GUI
 * @param LOGGER* log, Pointer for the logging service
 * @param master_start_stop* data, pointer for the data holder
 */
void start_all(GUI* g, LOGGER* log,struct master_start_stop_t* data);

#endif
