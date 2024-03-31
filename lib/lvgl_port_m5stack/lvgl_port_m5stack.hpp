#pragma once

#include <M5GFX.h>
#include <lvgl.h>

#ifndef LV_BUFFER_LINE
#define LV_BUFFER_LINE 120
#endif

void lvgl_port_init(M5GFX &gfx);
bool lvgl_port_lock(void);
void lvgl_port_unlock(void);
