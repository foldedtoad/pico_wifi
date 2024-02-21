# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(DTS_ROOT_BINDINGS "${CMAKE_CURRENT_SOURCE_DIR}")

set(BOARD rpi_pico_w)

set(EXTRA_CONF_FILE local.conf)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(app LANGUAGES C)


include_directories(app PRIVATE src)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE
  ${app_sources}
  )

add_subdirectory(../ build/drivers)

# zephyr_compile_options(-save-temps)
