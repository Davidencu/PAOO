cmake_minimum_required(VERSION 3.16)
project(Lucrarea5si6 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(lucrarea5si6
    src/main.cpp
)
