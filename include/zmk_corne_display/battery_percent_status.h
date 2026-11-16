/*
 * Copyright (c) 2026
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <lvgl.h>
#include <zephyr/kernel.h>

struct zmk_widget_battery_percent_status {
    sys_snode_t node;
    lv_obj_t *obj;
    lv_obj_t *icon;
};

int zmk_widget_battery_percent_status_init(struct zmk_widget_battery_percent_status *widget,
                                           lv_obj_t *parent);
lv_obj_t *zmk_widget_battery_percent_status_obj(struct zmk_widget_battery_percent_status *widget);
lv_obj_t *zmk_widget_battery_percent_status_icon_obj(
    struct zmk_widget_battery_percent_status *widget);
