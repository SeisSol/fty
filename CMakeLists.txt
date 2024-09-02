# SPDX-FileCopyrightText: 2020-2023 Ravil Dorozhinskii
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.5)
project(fty-converter)

target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_17)

find_package(YAML-CPP REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} converter.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${YAML_CPP_INCLUDE_DIR} ${PROJECT_SOURCE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${YAML_CPP_LIBRARIES})
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -std=c++14 -Wall -Wextra -pedantic -Werror)

