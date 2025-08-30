cmake_minimum_required(VERSION 3.24.0)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_SOURCE_DIR} "/../../PPG_PCG_ECG_synchro/Sensor_init"
                         ${CMAKE_CURRENT_SOURCE_DIR} "/../max30102_test/component)

project(inmp441_test)
