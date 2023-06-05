cmake_minimum_required(VERSION 3.25)
project(ptrace)

set(CMAKE_CXX_STANDARD 17)

set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
