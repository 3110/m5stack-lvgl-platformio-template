#include "lvgl_port_m5stack.hpp"

#if defined(USE_EMULATOR)
static SDL_mutex *xGuiMutex;

static uint32_t lvgl_tick_timer(uint32_t interval, void *param) {
    (void)interval;
    (void)param;
    lv_tick_inc(10);
    return 10;
}

static int lvgl_sdl_thread(void *data) {
    (void)data;
    while (1) {
        if (SDL_LockMutex(xGuiMutex) == 0) {
            lv_timer_handler();
            SDL_UnlockMutex(xGuiMutex);
        }
        SDL_Delay(10);
    }
    return 0;
}

static void lvgl_flush_cb(lv_display_t *disp, const lv_area_t *area,
                          uint8_t *px_map) {
    M5GFX &gfx = *(M5GFX *)lv_display_get_driver_data(disp);

    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    gfx.startWrite();
    gfx.setAddrWindow(area->x1, area->y1, w, h);
    gfx.writePixels((lgfx::rgb565_t *)px_map, w * h);
    gfx.endWrite();

    lv_display_flush_ready(disp);
}

static void lvgl_read_cb(lv_indev_t *indev, lv_indev_data_t *data) {
    M5GFX &gfx = *(M5GFX *)lv_indev_get_driver_data(indev);
    uint16_t touchX, touchY;

    bool touched = gfx.getTouch(&touchX, &touchY);
    if (!touched) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

void lvgl_port_init(M5GFX &gfx) {
    lv_init();

    static lv_display_t *disp = lv_display_create(gfx.width(), gfx.height());
    if (disp == NULL) {
        LV_LOG_ERROR("lv_display_create failed");
        return;
    }

    lv_display_set_driver_data(disp, &gfx);
    lv_display_set_flush_cb(disp, lvgl_flush_cb);

    static uint8_t *buf1 = (uint8_t *)malloc(gfx.width() * LV_BUFFER_LINE * 2);
    static uint8_t *buf2 = (uint8_t *)malloc(gfx.width() * LV_BUFFER_LINE * 2);
    lv_display_set_buffers(disp, (void *)buf1, (void *)buf2,
                           gfx.width() * LV_BUFFER_LINE * 2,
                           LV_DISPLAY_RENDER_MODE_PARTIAL);

    static lv_indev_t *indev = lv_indev_create();
    LV_ASSERT_MALLOC(indev);
    if (indev == NULL) {
        LV_LOG_ERROR("lv_indev_create failed");
        return;
    }
    lv_indev_set_driver_data(indev, &gfx);
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, lvgl_read_cb);
    lv_indev_set_display(indev, disp);

    xGuiMutex = SDL_CreateMutex();
    SDL_AddTimer(10, lvgl_tick_timer, NULL);
    SDL_CreateThread(lvgl_sdl_thread, "lvgl_sdl_thread", NULL);
}

bool lvgl_port_lock(void) {
    return SDL_LockMutex(xGuiMutex) == 0 ? true : false;
}

void lvgl_port_unlock(void) {
    SDL_UnlockMutex(xGuiMutex);
}
#else
static SemaphoreHandle_t xGuiSemaphore;

static void lvgl_tick_timer(void *arg) {
    (void)arg;
    lv_tick_inc(10);
}

static void lvgl_rtos_task(void *pvParameter) {
    (void)pvParameter;
    while (1) {
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
            lv_timer_handler();
            xSemaphoreGive(xGuiSemaphore);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

static void lvgl_flush_cb(lv_display_t *disp, const lv_area_t *area,
                          uint8_t *px_map) {
    M5GFX &gfx = *(M5GFX *)lv_display_get_driver_data(disp);

    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    gfx.startWrite();
    gfx.setAddrWindow(area->x1, area->y1, w, h);
    gfx.writePixels((lgfx::rgb565_t *)px_map, w * h);
    gfx.endWrite();

    lv_display_flush_ready(disp);
}

static void lvgl_read_cb(lv_indev_t *indev, lv_indev_data_t *data) {
    M5GFX &gfx = *(M5GFX *)lv_indev_get_driver_data(indev);
    uint16_t touchX, touchY;

    bool touched = gfx.getTouch(&touchX, &touchY);
    if (!touched) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

void lvgl_port_init(M5GFX &gfx) {
    lv_init();

    static lv_display_t *disp = lv_display_create(gfx.width(), gfx.height());
    if (disp == NULL) {
        LV_LOG_ERROR("lv_display_create failed");
        return;
    }

    lv_display_set_driver_data(disp, &gfx);
    lv_display_set_flush_cb(disp, lvgl_flush_cb);
    static uint8_t *buf1 = (uint8_t *)malloc(gfx.width() * LV_BUFFER_LINE);
    lv_display_set_buffers(disp, (void *)buf1, NULL,
                           gfx.width() * LV_BUFFER_LINE,
                           LV_DISPLAY_RENDER_MODE_PARTIAL);

    static lv_indev_t *indev = lv_indev_create();
    LV_ASSERT_MALLOC(indev);
    if (indev == NULL) {
        LV_LOG_ERROR("lv_indev_create failed");
        return;
    }
    lv_indev_set_driver_data(indev, &gfx);
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, lvgl_read_cb);
    lv_indev_set_display(indev, disp);

    xGuiSemaphore = xSemaphoreCreateMutex();
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lvgl_tick_timer, .name = "lvgl_tick_timer"};
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 10 * 1000));
    xTaskCreate(lvgl_rtos_task, "lvgl_rtos_task", 4096, NULL, 1, NULL);
}

bool lvgl_port_lock(void) {
    return xSemaphoreTake(xGuiSemaphore, portMAX_DELAY) == pdTRUE;
}

void lvgl_port_unlock(void) {
    xSemaphoreGive(xGuiSemaphore);
}
#endif
