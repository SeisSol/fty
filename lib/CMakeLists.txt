cmake_minimum_required(VERSION 3.5)
project(fty-lib)

find_package(YAML-CPP REQUIRED)

add_library(fty-lib FtyIO.cpp
                    FtyBlockParsing.cpp
                    FtyStringProcessing.cpp
                    FtyBlockProcessing.cpp
                    FtyLoader.cpp)

target_include_directories(fty-lib PUBLIC ${YAML_CPP_INCLUDE_DIR} ../include)
target_link_libraries(fty-lib PUBLIC ${YAML_CPP_LIBRARIES})
target_compile_options(fty-lib PRIVATE "-std=c++11")