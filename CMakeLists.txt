cmake_minimum_required(VERSION 3.16)
project(solution)

set(CMAKE_CXX_STANDARD 11)
# WorldCupTests.cpp catch.hpp
add_executable(solution main23a1.cpp worldcup23a1.cpp Player.cpp Player.h Team.cpp Team.h)
