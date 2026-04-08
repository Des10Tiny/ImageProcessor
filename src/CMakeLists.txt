add_library(image_processor_lib STATIC
    app_runner.cpp
    bmp_processor.cpp
    change_threads.cpp
    filter_factory.cpp
    generate_start_setup.cpp
    parameters.cpp
    validation_data.cpp
    filters/crop.cpp
    filters/edge_detection.cpp
    filters/gaussian_blur.cpp
    filters/grayscale.cpp
    filters/negative.cpp
    filters/sharpening.cpp
    filters/smoothing.cpp
)

if(MSVC)
    target_compile_options(image_processor_lib PRIVATE /W4 /WX)
else()
    target_compile_options(image_processor_lib PRIVATE -Wall -Wextra -Werror)
endif()

target_include_directories(image_processor_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)
add_executable(ImageProcessor main.cpp)
target_link_libraries(ImageProcessor PRIVATE image_processor_lib)
