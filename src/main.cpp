#include "main.hpp"

#include "lvgl_port_m5stack.hpp"

extern void user_app(void);

void setup(void) {
    M5_BEGIN();
    lvgl_port_init(M5_DISPLAY());
    if (lvgl_port_lock()) {
        user_app();
        lvgl_port_unlock();
    }
}

void loop(void) {
    M5_UPDATE();
    delay(10);
}
