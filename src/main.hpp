#pragma once

#if defined(ARDUINO_M5STACK_DIAL) && !defined(USE_EMULATOR)
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

#if defined(USE_EMULATOR)
#include <unistd.h>
inline void M5_DELAY(useconds_t usec) {
    usleep(usec * 1000);
}
#else
inline void M5_DELAY(uint32_t msec) {
    delay(msec);
}
#endif