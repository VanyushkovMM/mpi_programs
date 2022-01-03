cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(mpi)

set(main main)
set(max_vector_value max_vector_value)
set(trapez_method trapez_method)
set(vector_alternations vector_alternations)
set(dijkstra dijkstra)

find_package(MPI)
include_directories(${MPI_INCLUDE_PATH}) 
message(STATUS "Using MPI ${MPI_LIBRARIES}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/max_vector_value ${CMAKE_CURRENT_SOURCE_DIR}/trapez_method ${CMAKE_CURRENT_SOURCE_DIR}/vector_alternations ${CMAKE_CURRENT_SOURCE_DIR}/dijkstra)

add_subdirectory(main)
add_subdirectory(max_vector_value)
add_subdirectory(trapez_method)
add_subdirectory(vector_alternations)
add_subdirectory(dijkstra)

option(BTEST "build test?" OFF)

if(BTEST)
    add_subdirectory(gtest)
    add_subdirectory(test)
endif()

