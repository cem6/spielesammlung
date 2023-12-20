cmake_minimum_required(VERSION 3.27)
project(rakete)

set(CMAKE_CXX_STANDARD 17)

add_executable(rakete main.cpp)

target_link_libraries(rakete ncurses)