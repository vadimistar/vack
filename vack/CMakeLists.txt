cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(vack VERSION 0.1)

add_subdirectory("src")

target_include_directories(vack PUBLIC ${PROJECT_BINARY_DIR})
