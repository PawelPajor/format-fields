cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 20)
project(format-fields VERSION 0.0.1)

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else ()
    message(WARNING "The file ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif ()

include_directories(src/lib)
add_subdirectory(src)
