# PlatformIO IDEでLVGLを使ってM5Stack用プログラムを開発するための雛形

M5DialでLVGL v9のベンチマークデモを動かせるようにしてあります。

## 実装方法

基本的に`user_app()`を実装すれば動くようになっています。

### `main.cpp`
```c++
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
```

### `examples/lv_demo_benchmark/user_app.cpp`

```c++
#include <lvgl.h>
#include <demos/lv_demos.h>

void user_app(void) {
    lv_demo_benchmark();
}
```

## 参考

* [LVGL: Light and Versatile Graphics Library](https://github.com/lvgl/lvgl)
* [Running the M5Stack LVGL device emulator via PlatformIO](https://github.com/m5stack/lv_m5_emulator)
