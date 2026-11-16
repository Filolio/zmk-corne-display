/*
 * Copyright (c) 2026
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/battery.h>
#include <zmk/display.h>
#include <zmk/usb.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>

#include <zmk_corne_display/battery_percent_status.h>

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct battery_percent_status_state {
    uint8_t level;
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
    bool usb_present;
#endif
};

static void set_battery_symbol(lv_obj_t *label, struct battery_percent_status_state state) {
    const char *icon;

    if (state.level > 95) {
        icon = LV_SYMBOL_BATTERY_FULL;
    } else if (state.level > 65) {
        icon = LV_SYMBOL_BATTERY_3;
    } else if (state.level > 35) {
        icon = LV_SYMBOL_BATTERY_2;
    } else if (state.level > 5) {
        icon = LV_SYMBOL_BATTERY_1;
    } else {
        icon = LV_SYMBOL_BATTERY_EMPTY;
    }

    char text[24] = {};

#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
    if (state.usb_present) {
        snprintf(text, sizeof(text), "%s %s %u", LV_SYMBOL_CHARGE, icon, state.level);
    } else
#endif /* IS_ENABLED(CONFIG_USB_DEVICE_STACK) */
    {
        snprintf(text, sizeof(text), "%s %u", icon, state.level);
    }

    lv_label_set_text(label, text);
}

static void battery_percent_status_update_cb(struct battery_percent_status_state state) {
    struct zmk_widget_battery_percent_status *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_battery_symbol(widget->obj, state); }
}

static struct battery_percent_status_state
battery_percent_status_get_state(const zmk_event_t *eh) {
    const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);

    return (struct battery_percent_status_state){
        .level = (ev != NULL) ? ev->state_of_charge : zmk_battery_state_of_charge(),
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
        .usb_present = zmk_usb_is_powered(),
#endif /* IS_ENABLED(CONFIG_USB_DEVICE_STACK) */
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_percent_status, struct battery_percent_status_state,
                            battery_percent_status_update_cb, battery_percent_status_get_state)

ZMK_SUBSCRIPTION(widget_battery_percent_status, zmk_battery_state_changed);
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
ZMK_SUBSCRIPTION(widget_battery_percent_status, zmk_usb_conn_state_changed);
#endif /* IS_ENABLED(CONFIG_USB_DEVICE_STACK) */

int zmk_widget_battery_percent_status_init(struct zmk_widget_battery_percent_status *widget,
                                           lv_obj_t *parent) {
    widget->obj = lv_label_create(parent);

    /* Текст заряда всегда в одну строку, без переносов и поворотов. */
    lv_label_set_long_mode(widget->obj, LV_LABEL_LONG_CLIP);

    sys_slist_append(&widgets, &widget->node);

    widget_battery_percent_status_init();
    return 0;
}

lv_obj_t *zmk_widget_battery_percent_status_obj(struct zmk_widget_battery_percent_status *widget) {
    return widget->obj;
}
