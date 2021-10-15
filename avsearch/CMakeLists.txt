cmake_minimum_required(VERSION 3.20)
project(avsearch C)

set(CMAKE_C_STANDARD 99)

add_executable(avsearch main.c avsearch.h avsearch.c)
