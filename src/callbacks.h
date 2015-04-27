#ifndef CALLBACKS_H_
#define CALLBACKS_H_

void external_force_device_enable_callback(GUI* g, WIDGET* w,void* data);
void external_grip_device_enable_callback(GUI* g,WIDGET* w,void* data);
void module_enable_callback(GUI* g,WIDGET* w,void* data);
void force_displacement_button_group_callback(GUI* g,WIDGET* w,void* data);

void start_module_callback(GUI* g, WIDGET* w,void* data);
void stop_module_callback(GUI* g,WIDGET* w,void* data);

#endif

