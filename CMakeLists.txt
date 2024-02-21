# Copyright (c) 2023 Beechwoods Software, Inc.
# SPDX-License-Identifier: Apache-2.0
#
# This CMake file is picked by the Zephyr build system because it is defined
# as the module CMake entry point (see zephyr/module.yml).

#zephyr_include_directories()

set(DTS_ROOT_BINDINGS ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(drivers)
