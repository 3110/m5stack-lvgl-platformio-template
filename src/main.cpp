#include "main.hpp"

#include "lvgl_port_m5stack.hpp"

#if defined(LV_USE_DEMO_BENCHMARK)
#include <demos/lv_demos.h>
inline void user_app(void) {
    lv_demo_benchmark();
}
#else
inline void user_app(void) {
}
#endif

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
