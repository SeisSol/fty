cmake_minimum_required(VERSION 3.5)
project(testing)

find_package(YAML-CPP REQUIRED)
find_package(GTest REQUIRED)

add_executable(tests ./main.cpp ./StringProcessorTest.cpp)
target_compile_options(tests PRIVATE "-std=c++11")
target_include_directories(tests PRIVATE ${YAML_CPP_INCLUDE_DIR} ${GTEST_INCLUDE_DIRS} ../include)
target_link_libraries(tests PRIVATE ${YAML_CPP_LIBRARIES} ${GTEST_LIBRARIES})

enable_testing()
add_test(NAME tests COMMAND tests)