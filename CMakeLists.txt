cmake_minimum_required(VERSION 3.5)
project(fty-converter)

option(WITH_TESTS "build with unit-tests" OFF)


add_executable(${CMAKE_PROJECT_NAME} converter.cpp)
add_subdirectory(lib)
target_link_libraries(${CMAKE_PROJECT_NAME} fty-lib)
