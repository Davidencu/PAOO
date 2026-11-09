cmake_minimum_required(VERSION 3.16)
project(Lucrarea4 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(buffer_demo
    src/buffer_demo.cpp
)
