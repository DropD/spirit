cmake_minimum_required(VERSION 2.8)
project(test_project)

add_executable(tp main.cpp)

add_definitions(-BOOST_DISABLE_ASSERTS)

set(cmake_cxx_flags "")
