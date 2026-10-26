cmake_minimum_required(VERSION 3.10)
project(MathLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(mathlib STATIC src/mathlib.cpp)

target_include_directories(mathlib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

install(TARGETS mathlib ARCHIVE DESTINATION lib)
install(FILES include/mathlib.h DESTINATION include)
