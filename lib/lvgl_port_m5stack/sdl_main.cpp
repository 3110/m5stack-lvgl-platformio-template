#if defined(USE_EMULATOR)

#include <M5GFX.h>

extern void setup(void);
extern void loop(void);

__attribute__((weak)) int user_func(bool *running) {
    setup();
    do {
        loop();
    } while (*running);
    return 0;
}

int main(int, char **) {
    // The second argument is effective for step execution with breakpoints.
    // You can specify the time in milliseconds to perform slow execution that
    // ensures screen updates.
    return lgfx::Panel_sdl::main(user_func, 128);
}

#endif
