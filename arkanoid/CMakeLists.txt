cmake_minimum_required(VERSION 3.27)
project(arkanoid)

set(CMAKE_CXX_STANDARD 17)

add_executable(arkanoid main.cpp)

target_link_libraries(arkanoid ncurses)