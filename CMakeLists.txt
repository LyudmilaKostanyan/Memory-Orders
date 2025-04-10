cmake_minimum_required(VERSION 3.10)

project(Thread-Safe-Counter-with-Atomics)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_executable(main main.cpp)
