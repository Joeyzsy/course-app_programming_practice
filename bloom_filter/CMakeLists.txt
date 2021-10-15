cmake_minimum_required(VERSION 3.20)
project(bloom_filter_C C)

set(CMAKE_C_STANDARD 11)

add_executable(bloom_filter_C
        cmake-build-debug/MurmurHash2.h bloom.c bloom.h bf_search.c)
