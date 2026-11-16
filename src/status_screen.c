/*
 * Copyright (c) 2026
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>

#include <zmk/display/status_screen.h>
#include <zmk/display/widgets/layer_status.h>
#include <zmk/display/widgets/output_status.h>
#include <zmk/display/widgets/peripheral_status.h>
#include <zmk/display/widgets/wpm_status.h>

#include <zmk_corne_display/battery_percent_status.h>
#include <zmk_corne_display/display_rotation.h>

#if IS_ENABLED(CONFIG_ZMK_CORNE_DISPLAY_BATTERY_PERCENT)
static struct zmk_widget_battery_percent_status battery_percent_status_widget;
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_OUTPUT_STATUS)
static struct zmk_widget_output_status output_status_widget;
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_PERIPHERAL_STATUS)
static struct zmk_widget_peripheral_status peripheral_status_widget;
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_LAYER_STATUS)
static struct zmk_widget_layer_status layer_status_widget;
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_WPM_STATUS)
static struct zmk_widget_wpm_status wpm_status_widget;
#endif

lv_obj_t *zmk_display_status_screen(void) {
#if IS_ENABLED(CONFIG_ZMK_CORNE_DISPLAY_ROTATED)
    zmk_corne_display_rotation_init();
#endif

    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_style_pad_all(screen, 0, LV_PART_MAIN);

#if IS_ENABLED(CONFIG_ZMK_CORNE_DISPLAY_ROTATED)
    const lv_font_t *status_font = lv_theme_get_font_small(screen);
#if IS_ENABLED(CONFIG_LV_FONT_MONTSERRAT_10)
    status_font = &lv_font_montserrat_10;
#endif
#endif

#if IS_ENABLED(CONFIG_ZMK_CORNE_DISPLAY_BATTERY_PERCENT)
    zmk_widget_battery_percent_status_init(&battery_percent_status_widget, screen);
    lv_obj_t *battery_value = zmk_widget_battery_percent_status_obj(&battery_percent_status_widget);
    lv_obj_t *battery_icon =
        zmk_widget_battery_percent_status_icon_obj(&battery_percent_status_widget);

    lv_obj_align(battery_value, LV_ALIGN_TOP_RIGHT, 0, 0);
#if IS_ENABLED(CONFIG_ZMK_CORNE_DISPLAY_ROTATED)
    lv_obj_align(battery_icon, LV_ALIGN_TOP_RIGHT, 0, 18);
#else
    lv_obj_align_to(battery_icon, battery_value, LV_ALIGN_OUT_LEFT_MID, -1, 0);
#endif
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_OUTPUT_STATUS)
    zmk_widget_output_status_init(&output_status_widget, screen);
    lv_obj_t *output_status = zmk_widget_output_status_obj(&output_status_widget);
#if IS_ENABLED(CONFIG_ZMK_CORNE_DISPLAY_ROTATED)
    lv_obj_set_style_text_font(output_status, status_font, LV_PART_MAIN);
    lv_label_set_long_mode(output_status, LV_LABEL_LONG_CLIP);
    lv_obj_align(output_status, LV_ALIGN_TOP_LEFT, 0, 46);
#else
    lv_obj_align(output_status, LV_ALIGN_TOP_LEFT, 0, 0);
#endif
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_PERIPHERAL_STATUS)
    zmk_widget_peripheral_status_init(&peripheral_status_widget, screen);
    lv_obj_t *peripheral_status = zmk_widget_peripheral_status_obj(&peripheral_status_widget);
#if IS_ENABLED(CONFIG_ZMK_CORNE_DISPLAY_ROTATED)
    lv_obj_set_style_text_font(peripheral_status, status_font, LV_PART_MAIN);
    lv_label_set_long_mode(peripheral_status, LV_LABEL_LONG_CLIP);
    lv_obj_align(peripheral_status, LV_ALIGN_TOP_LEFT, 0, 46);
#else
    lv_obj_align(peripheral_status, LV_ALIGN_TOP_LEFT, 0, 0);
#endif
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_LAYER_STATUS)
    zmk_widget_layer_status_init(&layer_status_widget, screen);
    lv_obj_t *layer_status = zmk_widget_layer_status_obj(&layer_status_widget);
#if IS_ENABLED(CONFIG_ZMK_CORNE_DISPLAY_ROTATED)
    lv_obj_set_style_text_font(layer_status, status_font, LV_PART_MAIN);
    lv_label_set_long_mode(layer_status, LV_LABEL_LONG_CLIP);
#else
    lv_obj_set_style_text_font(layer_status, lv_theme_get_font_small(screen), LV_PART_MAIN);
#endif
    lv_obj_align(layer_status, LV_ALIGN_BOTTOM_LEFT, 0, 0);
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_WPM_STATUS)
    zmk_widget_wpm_status_init(&wpm_status_widget, screen);
    lv_obj_align(zmk_widget_wpm_status_obj(&wpm_status_widget), LV_ALIGN_BOTTOM_RIGHT, 0, 0);
#endif

    return screen;
}
