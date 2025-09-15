cmake_minimum_required(VERSION 3.23.2)
set(CMAKE_C_STANDARD 11)
project(receiver)
add_executable(receiver main.c)
target_link_libraries(receiver PRIVATE rt)

