/*
 * Copyright (c) 2026
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/logging/log.h>

#include <lvgl.h>

#include <string.h>

#include <zmk_corne_display/display_rotation.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define DISPLAY_NODE DT_CHOSEN(zephyr_display)

#define NATIVE_WIDTH DT_PROP(DISPLAY_NODE, width)
#define NATIVE_HEIGHT DT_PROP(DISPLAY_NODE, height)

/* Логический экран — это нативная панель, повернутая на 90 градусов. */
#define LOGICAL_WIDTH NATIVE_HEIGHT
#define LOGICAL_HEIGHT NATIVE_WIDTH

static const struct device *const display_dev = DEVICE_DT_GET(DISPLAY_NODE);

/*
 * Готовый кадр в нативном формате SSD1306: 128x32, по одному байту на
 * вертикальную колонку из 8 пикселей.
 */
static uint8_t native_buf[(NATIVE_WIDTH * NATIVE_HEIGHT) / 8];

static inline void set_native_pixel(uint16_t x, uint16_t y, bool on) {
    uint8_t *byte = &native_buf[x + (y / 8) * NATIVE_WIDTH];
    uint8_t mask = BIT(y % 8);

    if (on) {
        *byte |= mask;
    } else {
        *byte &= (uint8_t)~mask;
    }
}

/*
 * LVGL отдает область в упакованном монохромном виде (set_px_cb из
 * zephyr/modules/lvgl): байт на каждые 8 пикселей по вертикали,
 * шаг строки равен ширине области в пикселях.
 */
static void rotated_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p) {
    const uint16_t width = lv_area_get_width(area);
    const uint16_t height = lv_area_get_height(area);
    const uint8_t *src = (const uint8_t *)color_p;

    for (uint16_t y = 0; y < height; y++) {
        const uint8_t *row = src + (y / 8) * width;

        for (uint16_t x = 0; x < width; x++) {
            bool on = (row[x] >> (y % 8)) & 1;

            uint16_t lx = area->x1 + x;
            uint16_t ly = area->y1 + y;

            /*
             * Панель установлена вертикально, повернута на 90 градусов
             * против часовой стрелки. Поэтому кадр разворачиваем по
             * часовой: логический верх и низ уходят в нативную ось X.
             */
            uint16_t nx = NATIVE_WIDTH - 1 - ly;
            uint16_t ny = lx;

            set_native_pixel(nx, ny, on);
        }
    }

    if (lv_disp_flush_is_last(drv)) {
        struct display_buffer_descriptor desc = {
            .buf_size = sizeof(native_buf),
            .width = NATIVE_WIDTH,
            .pitch = NATIVE_WIDTH,
            .height = NATIVE_HEIGHT,
        };

        if (display_write(display_dev, 0, 0, &desc, native_buf) != 0) {
            LOG_ERR("Failed to write rotated display buffer");
        }
    }

    lv_disp_flush_ready(drv);
}

void zmk_corne_display_rotation_init(void) {
    static bool initialized;

    if (initialized) {
        return;
    }

    lv_disp_t *disp = lv_disp_get_default();
    if (disp == NULL || disp->driver == NULL) {
        LOG_ERR("LVGL display is not available");
        return;
    }

    if (!device_is_ready(display_dev)) {
        LOG_ERR("Display device is not ready");
        return;
    }

    memset(native_buf, 0, sizeof(native_buf));

    lv_disp_drv_t *drv = disp->driver;

    /* Упаковку пикселей и выравнивание оставляем штатные (монохромные). */
    drv->hor_res = LOGICAL_WIDTH;
    drv->ver_res = LOGICAL_HEIGHT;
    drv->flush_cb = rotated_flush_cb;

    lv_disp_drv_update(disp, drv);

    initialized = true;
}
