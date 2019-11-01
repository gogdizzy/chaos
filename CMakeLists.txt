cmake_minimum_required(VERSION 3.10)

project(chaos)

set(CMAKE_CXX_STANDARD 14)

message(STATUS "operation system is ${CMAKE_SYSTEM}")

add_compile_options(-O2)

add_subdirectory(chaos/unit-test)
add_subdirectory(chaos/priority-queue)
add_subdirectory(chaos/hash-map)
add_subdirectory(chaos/template-study)
add_subdirectory(chaos/rtree)
