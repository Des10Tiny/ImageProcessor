cmake_minimum_required(VERSION 3.5)
project(ImageProcessor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(CMAKE_EXPORT_COMPILE_COMMANDS AND UNIX)
        execute_process(
                COMMAND ${CMAKE_COMMAND} -E create_symlink
                ${CMAKE_BINARY_DIR}/compile_commands.json
                ${CMAKE_SOURCE_DIR}/compile_commands.json
        )
endif()

add_subdirectory(src)

option(BUILD_TESTS "Build all unit tests" ON)

if(CMAKE_BUILD_TYPE MATCHES "Release")
        set(BUILD_TESTS OFF)
        cmake_policy(SET CMP0069 NEW)
        include(CheckIPOSupported)
        check_ipo_supported(RESULT ipo_supported OUTPUT error)

        if(ipo_supported)
                set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
        endif()

endif()

if(BUILD_TESTS)
        enable_testing()
        add_subdirectory(tests)
endif()

