cmake_minimum_required(VERSION 3.0)
project(lvalue_to_rvalue)
set(CMAKE_CXX_STANDARD 11)
add_compile_options(-fno-elide-constructors)
add_executable(lvalue_to_rvalue main.cpp)