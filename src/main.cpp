#if defined(ARDUINO_M5STACK_DIAL)
#include <M5Dial.h>
inline void M5_BEGIN(bool enableEncoder = false, bool enableRFID = false) {
    M5Dial.begin(enableEncoder, enableRFID);
}
inline void M5_BEGIN(m5::M5Unified::config_t cfg, bool enableEncoder = false,
                     bool enableRFID = false) {
    M5Dial.begin(cfg, enableEncoder, enableRFID);
}
inline void M5_UPDATE(void) {
    M5.update();
}
inline M5GFX& M5_DISPLAY(void) {
    return M5Dial.Display;
}
#else
#include <M5Unified.h>
inline void M5_BEGIN(void) {
    M5.begin();
}
inline void M5_UPDATE(void) {
    M5.update();
}
inline M5GFX& M5_DISPLAY(void) {
    return M5.Display;
}
#endif

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
