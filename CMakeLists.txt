cmake_minimum_required(VERSION 3.27)
project(miniaudio_test C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(miniaudio_test main.c miniaudio.h)