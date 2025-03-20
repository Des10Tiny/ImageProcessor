cmake_minimum_required(VERSION 3.30)
project(ImageProcessor)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address ")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -fno-omit-frame-pointer")
set(CMAKE_CXX_STANDARD 20)

add_executable(ImageProcessor
        include/parameters.h
        include/filters/crop.h
        include/filter_base.h
        include/filter_factory.h
        include/bmp_processor.h
        include/validation_data.h
        include/generate_start_setup.h
        include/filters/grayscale.h
        include/filters/negative.h
        include/validation_exception.h
        include/filters/sharpening.h
        include/change_threads.h
        include/filters/edge_detection.h
        include/filters/gaussian_blur.h
        src/main.cpp
        src/parameters.cpp
        src/filters/crop.cpp
        src/bmp_processor.cpp
        src/filter_factory.cpp
        src/validation_data.cpp
        src/generate_start_setup.cpp
        src/filters/grayscale.cpp
        src/filters/negative.cpp
        src/filters/sharpening.cpp
        src/change_threads.cpp
        src/filters/edge_detection.cpp
        src/filters/gaussian_blur.cpp
)

