cmake_minimum_required(VERSION 3.23.2)
project(app)
add_executable(app main.c)

find_package(Threads REQUIRED)
target_link_libraries(app PRIVATE Threads::Threads)