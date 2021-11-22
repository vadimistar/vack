cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(vkdisasm VERSION 0.1)

add_subdirectory("src")

if (NOT MSVC) 
  target_compile_options(vkdisasm PRIVATE -Wall -Wextra -Wshadow -Wunreachable-code -Wpedantic -fno-exceptions)
endif()

target_include_directories(vkdisasm PUBLIC ${PROJECT_BINARY_DIR})
