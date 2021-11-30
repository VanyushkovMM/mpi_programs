cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(thread)

set(main main)
set(max_vector_value max_vector_value)
set(trapez_method trapez_method)
set(vector_alternations vector_alternations)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/max_vector_value ${CMAKE_CURRENT_SOURCE_DIR}/trapez_method ${CMAKE_CURRENT_SOURCE_DIR}/vector_alternations)

add_subdirectory(main)
add_subdirectory(max_vector_value)
add_subdirectory(trapez_method)
add_subdirectory(vector_alternations)

option(BTEST "build test?" OFF)

if(BTEST)
    add_subdirectory(gtest)
    add_subdirectory(test)
endif()

