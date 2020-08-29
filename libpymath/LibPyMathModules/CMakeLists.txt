cmake_minimum_required(VERSION 3.16)
project(LibPyMathModules C)

set(CMAKE_C_STANDARD 99)

add_executable(LibPyMathModules main.c)

include_directories(../../../../../../AppData/Local/Programs/Python/Python38/include)