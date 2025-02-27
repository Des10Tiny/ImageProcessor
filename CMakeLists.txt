cmake_minimum_required(VERSION 3.30)
project(ImageProcessor)

set(CMAKE_CXX_STANDARD 20)

add_executable(ImageProcessor src/main.cpp
        src/parameters.cpp
        include/parameters.h
)
