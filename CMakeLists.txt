cmake_minimum_required(VERSION 3.30)
project(ImageProcessor)

set(CMAKE_CXX_STANDARD 20)

add_executable(ImageProcessor src/main.cpp
        src/parameters.cpp
        include/parameters.h
        include/imports.h
        include/filters/crop_filter.h
        include/filter_base.h
        src/filters/crop_filter.cpp
        include/bmp_processor.h
        src/bmp_processor.cpp
        include/filter_factory.h
        src/filter_factory.cpp
        src/validation_data.cpp
        include/validation_data.h
        src/generate_start_setup.cpp
        include/generate_start_setup.h
)
