cmake_minimum_required(VERSION 3.16)
project(solution)

set(CMAKE_CXX_STANDARD 14)

add_executable(solution main.cpp Player.cpp Player.h Team.cpp Team.h)
