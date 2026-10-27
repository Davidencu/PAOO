cmake_minimum_required(VERSION 3.16)
project(TemaPOO CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directoarele cu headere
include_directories(include)

# Sursele aplicației
add_executable(tema_app
    src/main.cpp
    src/DynamicArray.cpp
)