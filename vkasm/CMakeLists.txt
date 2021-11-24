cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(vkasm VERSION 0.1)

add_subdirectory("src")

target_include_directories(vkasm PUBLIC ${PROJECT_BINARY_DIR})

add_subdirectory("tests")
