cmake_minimum_required(VERSION 3.10)

project(chaos)

set(CMAKE_CXX_STANDARD 14)

message(STATUS "operation system is ${CMAKE_SYSTEM}")

add_subdirectory(chaos/unit-test)
