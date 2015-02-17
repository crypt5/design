#ifndef GRAPHICS_BUTTON_H_
#define GRAPHICS_BUTTON_H_

#include "graphics_widget.h"

WIDGET* create_button(char* text, int x, int y);
void destroy_button(WIDGET* w);

#endif
