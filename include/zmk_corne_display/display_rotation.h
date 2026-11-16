/*
 * Copyright (c) 2026
 * SPDX-License-Identifier: MIT
 */

#pragma once

/**
 * Переключает LVGL на логический экран 32x128 и поворачивает кадр
 * в нативные 128x32 при отправке на вертикально установленный OLED.
 *
 * Должно вызываться до создания статус-экрана, после инициализации
 * дисплея ZMK.
 */
void zmk_corne_display_rotation_init(void);
