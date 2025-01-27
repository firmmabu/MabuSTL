cmake_minimum_required(VERSION 3.0)
project(MabuSTL)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        MabuSTL main.cpp
        mabu_stddef.h
        mabu_functional.h
        mabu_type_traits.h
        mabu_iterator.h
        mabu_utility.h
        mabu_algorithm_base.h
        mabu_construct.h
        mabu_heap_algorithm.h
        mabu_set_algorithm.h
        mabu_numeric.h
)
