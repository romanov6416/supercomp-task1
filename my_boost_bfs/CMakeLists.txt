cmake_minimum_required(VERSION 3.6)   # CMake version check
project(my_boost_bfs)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard
set(MY_SOURCE_FILES main.cpp)            # Add main.cpp file of project root directory as source file
set(I_SOURCE_FILES i_boost_bfs.cpp)            # Add main.cpp file of project root directory as source file
set(P_SOURCE_FILES tmp.cpp)            # Add main.cpp file of project root directory as source file

add_executable(my_boost_bfs ${MY_SOURCE_FILES})       # Add executable target with source files listed in SOURCE_FILES variable
add_executable(i_boost_bfs ${I_SOURCE_FILES})       # Add executable target with source files listed in SOURCE_FILES variable
add_executable(p_boost_bfs ${P_SOURCE_FILES})       # Add executable target with source files listed in SOURCE_FILES variable
