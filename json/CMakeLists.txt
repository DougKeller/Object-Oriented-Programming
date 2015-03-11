project(json CXX)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-Wall -Werror -std=c++11")

add_executable(json json.hpp json.cpp main.cpp)
