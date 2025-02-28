cmake_minimum_required(VERSION 3.30)
project(ImageProcessor)

set(CMAKE_CXX_STANDARD 20)

add_executable(ImageProcessor
        include/parameters.h
        include/imports.h
        include/filters/crop_filter.h
        include/filter_base.h
        include/filter_factory.h
        include/bmp_processor.h
        include/validation_data.h
        include/generate_start_setup.h
        src/main.cpp
        src/parameters.cpp
        src/filters/crop_filter.cpp
        src/bmp_processor.cpp
        src/filter_factory.cpp
        src/validation_data.cpp
        src/generate_start_setup.cpp

)
