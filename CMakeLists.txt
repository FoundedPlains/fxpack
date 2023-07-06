cmake_minimum_required(VERSION 3.22)
project(fxpack VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -O2 -g -Wall -Wextra -pedantic")

add_library(toml STATIC ./libs/tomlc99/toml.h ./libs/tomlc99/toml.c)

file(GLOB_RECURSE SRC ./src/*.cpp ./src/*.c ./src/*.h ./src/*.hpp)

include_directories(./libs/tomlc99/)

add_executable(fxpack ${SRC})
target_link_libraries(fxpack toml)
