cmake_minimum_required(VERSION 4.3)
project(rsa C)

set(CMAKE_C_STANDARD 23)

add_executable(rsa main.c)
target_link_libraries(rsa gmp)