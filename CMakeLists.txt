# Copyright (c) 2026
# SPDX-License-Identifier: MIT

if(CONFIG_ZMK_CORNE_DISPLAY)
  zephyr_library()
  zephyr_library_sources(src/battery_percent_status.c)
  zephyr_library_sources(src/status_screen.c)
  zephyr_include_directories(include)
endif()
