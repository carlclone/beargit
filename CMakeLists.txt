# cmake_minimum_required(VERSION <specify CMake version here>)
project(beargit C)

set(CMAKE_C_STANDARD 11)

include_directories(hw1)

add_executable(beargit
        hw1/beargit.c
        hw1/beargit.h
        hw1/main.c
        hw1/util.h)
