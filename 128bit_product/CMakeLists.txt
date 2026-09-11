cmake_minimum_required(VERSION 4.3)
project(128bit_product C)

set(CMAKE_C_STANDARD 23)

add_executable(128bit_product main.c)
target_compile_options(128bit_product PRIVATE -mbmi2)