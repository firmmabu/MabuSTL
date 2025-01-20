cmake_minimum_required(VERSION 3.30)
project(MabuSTL)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        MabuSTL main.cpp
        mabustddef.h
        mabu_functional.h
        mabu_type_traits.h
        mabu_iterator.h
)
