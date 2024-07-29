# SPDX-FileCopyrightText: 2020-2023 Ravil Dorozhinskii
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.5)
project(testing)

find_package(YAML-CPP REQUIRED)
find_package(GTest REQUIRED)

add_executable(tests ./main.cpp
                     ./helper.cpp
                     ./StringProcessorTest.cpp
                     ./PolicyTest.cpp
                     ./BlockParserTest.cpp
                     ./BlockProcessorTest.cpp)


target_compile_options(tests PRIVATE "-std=c++14")
target_include_directories(tests PRIVATE ${YAML_CPP_INCLUDE_DIR} ${GTEST_INCLUDE_DIRS} ../include)
target_link_libraries(tests PRIVATE ${YAML_CPP_LIBRARIES} ${GTEST_LIBRARIES})
target_compile_options(tests PRIVATE -Wall -Wextra -pedantic -Werror)

enable_testing()
add_test(NAME tests COMMAND tests)