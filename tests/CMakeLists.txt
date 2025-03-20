cmake_minimum_required(VERSION 3.14)
project(ImageProcessorTests)


# Подключение Google Test через FetchContent
include(CTest)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

# Подключение основного проекта
add_subdirectory(.. ${CMAKE_BINARY_DIR}/ImageProcessor)

# Создание тестового исполняемого файла
add_executable(ImageProcessorTests
        test_main.cpp
        test_bmp_processor.cpp
        ../include/parameters.h
        ../include/filters/crop.h
        ../include/filter_base.h
        ../include/filter_factory.h
        ../include/bmp_processor.h
        ../include/validation_data.h
        ../include/generate_start_setup.h
        ../include/filters/grayscale.h
        ../include/filters/negative.h
        ../include/validation_exception.h
        ../include/filters/sharpening.h
        ../include/filters/edge_detection.h
        ../include/filters/gaussian_blur.h
        ../src/parameters.cpp
        ../src/filters/crop.cpp
        ../src/bmp_processor.cpp
        ../src/filter_factory.cpp
        ../src/validation_data.cpp
        ../src/generate_start_setup.cpp
        ../src/filters/grayscale.cpp
        ../src/filters/negative.cpp
        ../src/filters/sharpening.cpp
        ../src/filters/edge_detection.cpp
        ../src/filters/gaussian_blur.cpp
)



# Определение флага для тестов
target_compile_definitions(ImageProcessorTests PRIVATE UNIT_TEST)

# Подключение Google Test
target_link_libraries(ImageProcessorTests gtest_main gtest)

# Добавление тестов в CTest
include(GoogleTest)
gtest_discover_tests(ImageProcessorTests)