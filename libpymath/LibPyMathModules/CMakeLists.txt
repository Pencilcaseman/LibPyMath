cmake_minimum_required(VERSION 3.16)
project(LibPyMathModules C)

set(CMAKE_C_STANDARD 99)

add_executable(LibPyMathModules main.c)

include_directories(../../)
include_directories(../../../../../../../penci/anaconda3/pkgs/python-3.8.3-he1778fa_2/include)
