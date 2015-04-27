#ifndef DATA_STRUCTURES_H_
#define DATA_STRUCTURES_H_

struct gui_module_widgets_t{
  WIDGET* border;
  WIDGET* seperator1;
  WIDGET* seperator2;
  WIDGET* seperator3;
  WIDGET* enable_device;
  WIDGET* mode_label;
  WIDGET* force_mode;
  WIDGET* displacement_mode;
  WIDGET* desired_section_label;
  WIDGET* desired_unit;
  WIDGET* desired_label;
  WIDGET* desired_enter;
  WIDGET* start_device;
  WIDGET* stop_device;
  WIDGET* current_section_label;
  WIDGET* current_output_label;
  WIDGET* current_output;
  WIDGET* current_output_unit;
  WIDGET* device_status;
};

struct gui_extern_force_widgets_t{
  WIDGET* border;
  WIDGET* seperator;
  WIDGET* enable_device;
  WIDGET* output_label;
  WIDGET* output_display;
  WIDGET* output_unit;
};

struct gui_extern_grip_widgets_t{
  WIDGET* border;
  WIDGET* seperator;
  WIDGET* enable_device;
  WIDGET* displacement_label;
  WIDGET* displacement_output;
  WIDGET* displacement_unit;
  WIDGET* force_label;
  WIDGET* force_output;
  WIDGET* force_unit;
};

struct gui_start_stop_status_t{
  WIDGET* start_test;
  WIDGET* stop_test;
  WIDGET* file_display_label;
  WIDGET* status_label;
  WIDGET* status;
  WIDGET* file_display;
};

#endif
