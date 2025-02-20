# Copyright 2023 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.20)

# Set project name to demos
project(demos VERSION 0.0.1 LANGUAGES CXX)

# Import the libhal-lpc40 library/package (and all of its dependencies)
find_package(libhal-lpc40 REQUIRED CONFIG)

# List of demo applications
# To add a new demo to the list, simply add its filename without the .cpp
# extension to this list.
set(DEMOS adc blinker can gpio i2c interrupt_pin uart pwm spi)

add_library(startup_code main.cpp)
target_compile_options(startup_code PRIVATE -g)
target_link_libraries(startup_code PRIVATE libhal::lpc40)

foreach(demo IN LISTS DEMOS)
    set(current_project ${demo}.elf)
    message(STATUS "Generating Demo for \"${current_project}\"")
    add_executable(${current_project} applications/${demo}.cpp)

    target_compile_features(${current_project} PRIVATE cxx_std_20)
    target_compile_options(${current_project} PRIVATE -g)
    target_link_options(${current_project} PRIVATE
        --oslib=semihost --crt0=minimal)
    target_link_libraries(${current_project} PRIVATE startup_code libhal::lpc40)

    libhal_post_build(${current_project})
    libhal_disassemble(${current_project})
endforeach()
