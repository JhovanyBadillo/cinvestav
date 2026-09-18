cmake_minimum_required(VERSION 4.3)
project(fifo_lifo_structures C)

set(CMAKE_C_STANDARD 23)

add_executable(fifo_lifo_structures main.c
        stack.h
        stack.c
        node.h
        node.c
        queue.h
        queue.c)
