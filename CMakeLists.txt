cmake_minimum_required(VERSION 2.8)
project (parser)

add_executable(ppars parser.cpp)
#add_executable(ppars2 parameter_parser.cpp)

add_definitions(-BOOST_DISABLE_ASSERTS)

set(cmake_cxx_flags "")
