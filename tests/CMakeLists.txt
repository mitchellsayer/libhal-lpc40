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

cmake_minimum_required(VERSION 3.15)

project(unit_test VERSION 0.0.1 LANGUAGES CXX)

if(DEFINED ENABLE_ASAN)
  set(ASAN_FLAG "-fsanitize=address" CACHE INTERNAL "Use AddressSanatizer")
endif(DEFINED ENABLE_ASAN)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # require at least gcc 11
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11.0)
    message(FATAL_ERROR "GCC version must be at least 11!")
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # require at least clang 14
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14)
    message(FATAL_ERROR "Clang version must be at least 14!")
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  # require at least clang 14
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14)
    message(FATAL_ERROR "Clang version must be at least 14!")
  endif()
else()
  message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC, detected ${CMAKE_CXX_COMPILER_ID}.")
endif()

find_package(ut REQUIRED CONFIG)
find_package(libhal REQUIRED CONFIG)
find_package(libhal-armcortex REQUIRED CONFIG)
find_package(libhal-util REQUIRED CONFIG)
find_package(ring-span-lite REQUIRED CONFIG)

add_executable(unit_test

  # Sources
  ../src/adc.cpp
  ../src/can.cpp
  ../src/clock.cpp
  ../src/i2c.cpp
  ../src/input_pin.cpp
  ../src/interrupt_pin.cpp
  ../src/output_pin.cpp
  ../src/pin.cpp
  ../src/power.cpp
  ../src/pwm.cpp
  ../src/uart.cpp
  ../src/spi.cpp

  # Tests
  adc.test.cpp
  input_pin.test.cpp
  output_pin.test.cpp
  interrupt_pin.test.cpp
  can.test.cpp
  uart.test.cpp
  i2c.test.cpp
  pwm.test.cpp
  spi.test.cpp
  main.test.cpp
)

target_include_directories(unit_test PUBLIC . ../include ../src)
target_compile_options(unit_test PRIVATE
  --coverage
  -fprofile-arcs
  -ftest-coverage
  ${ASAN_FLAG}
  -Werror
  -Wall
  -Wextra
  -Wshadow
  -Wnon-virtual-dtor
  -Wno-gnu-statement-expression
  -pedantic)

target_compile_features(unit_test PRIVATE cxx_std_20)

target_link_options(unit_test PRIVATE
  --coverage
  -fprofile-arcs
  -ftest-coverage
  ${ASAN_FLAG})

target_link_libraries(unit_test PRIVATE
  boost-ext-ut::ut
  libhal::libhal
  libhal::armcortex
  libhal::util
  nonstd::ring-span-lite)
