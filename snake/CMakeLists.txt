cmake_minimum_required(VERSION 3.27)
project(snake)

set(CMAKE_CXX_STANDARD 17)

add_executable(snake main.cpp)

target_link_libraries(snake ncurses)