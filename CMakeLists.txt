# Copyright (c) 2026
# SPDX-License-Identifier: MIT

if(CONFIG_ZMK_CORNE_DISPLAY)
  zephyr_library()
  zephyr_library_sources(src/battery_percent_status.c)
  zephyr_library_sources(src/display_rotation.c)
  zephyr_library_sources(src/status_screen.c)
  # ZMK добавляет свой app/include только таргету app, поэтому модулю
  # нужно подключить пути к заголовкам ZMK явно.
  zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)
  zephyr_library_include_directories(include)
endif()
