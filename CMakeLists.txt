# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly

cmake_minimum_required(VERSION 3.24.0)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set(EXTRA_COMPONENT_DIRS "D:/Esp-idf/PPG_PCG_ECG_ESP32/PPG_PCG_ECG_ESP32/component")
project(inmp441_test)
