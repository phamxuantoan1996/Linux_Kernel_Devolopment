cmake_minimum_required(VERSION 3.23.2)
set(CMAKE_C_STANDARD 11)
project(reader)

# Find pthread library
find_package(Threads REQUIRED)

add_executable(reader main.c)

# Link pthread and rt to executable
target_link_libraries(reader PRIVATE Threads::Threads rt) #refers to the real-time extensions library,