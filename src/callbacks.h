#ifndef CALLBACKS_H_
#define CALLBACKS_H_

/**
 * This is the callback to enable the external force device
 * @param GUI* g, pointer to the GUI
 * @param WIDGET* w, a pointer to the widget that generated the event
 * @param void* data, a pointer to the data needed buy the widget
 */
void external_force_device_enable_callback(GUI* g, WIDGET* w,void* data);

/**
 * This is the callback to enable the external grip device
 * @param GUI* g, pointer to the GUI
 * @param WIDGET* w, a pointer to the widget that generated the event
 * @param void* data, a pointer to the data needed buy the widget
 */
void external_grip_device_enable_callback(GUI* g,WIDGET* w,void* data);

/**
 * This is the callback to enable the motor module
 * @param GUI* g, pointer to the GUI
 * @param WIDGET* w, a pointer to the widget that generated the event
 * @param void* data, a pointer to the data needed buy the widget
 */
void module_enable_callback(GUI* g,WIDGET* w,void* data);

/**
 * This is the callback to simulate a button group for the force displacement
 * radio buttons
 * @param GUI* g, pointer to the GUI
 * @param WIDGET* w, a pointer to the widget that generated the event
 * @param void* data, a pointer to the data needed buy the widget
 */
void force_displacement_button_group_callback(GUI* g,WIDGET* w,void* data);

/**
 * This is the callback to start the motor module
 * @param GUI* g, pointer to the GUI
 * @param WIDGET* w, a pointer to the widget that generated the event
 * @param void* data, a pointer to the data needed buy the widget
 */
void start_module_callback(GUI* g, WIDGET* w,void* data);

/**
 * This is the callback to stop the motor module
 * @param GUI* g, pointer to the GUI
 * @param WIDGET* w, a pointer to the widget that generated the event
 * @param void* data, a pointer to the data needed buy the widget
 */
void stop_module_callback(GUI* g,WIDGET* w,void* data);

/**
 * This is the callback to start all enabled modules and
 * start the data logging
 * @param GUI* g, pointer to the GUI
 * @param WIDGET* w, a pointer to the widget that generated the event
 * @param void* data, a pointer to the data needed buy the widget
 */
void master_start_callback(GUI* g,WIDGET* w,void *data);

/**
 * This is the callback to stop all running devices and data collection
 * @param GUI* g, pointer to the GUI
 * @param WIDGET* w, a pointer to the widget that generated the event
 * @param void* data, a pointer to the data needed buy the widget
 */
void master_stop_callback(GUI* g,WIDGET* w,void *data);
#endif

