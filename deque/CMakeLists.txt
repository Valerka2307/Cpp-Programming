cmake_minimum_required(VERSION 3.12) # version can be different

project(my_cpp_project) #name of your project

add_subdirectory(googletest) # add googletest subdirectory

include_directories(googletest/include) # this is so we can #include <gtest/gtest.h>

add_compile_options(-std=c++20)

add_link_options(-std=c++20)

add_executable(mytests tests.cpp deque.hpp) # add this executable

target_link_libraries(mytests PRIVATE gtest) # link google test to this executable