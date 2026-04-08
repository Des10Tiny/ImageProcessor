include(FetchContent)


fetchcontent_declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
fetchcontent_makeavailable(googletest)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_executable(ImageProcessorTests
        test_bmp_processor.cpp
)

target_compile_definitions(ImageProcessorTests PRIVATE UNIT_TEST)
target_link_libraries(ImageProcessorTests
        PRIVATE
        image_processor_lib
        GTest::gtest_main
        GTest::gtest
)

if(MSVC)
        target_compile_options(image_processor_lib PRIVATE /W4 /WX)
else()
        target_compile_options(image_processor_lib PRIVATE -Wall -Wextra -Werror)
endif()

include(GoogleTest)
gtest_discover_tests(ImageProcessorTests)
