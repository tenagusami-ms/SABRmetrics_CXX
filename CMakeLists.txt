cmake_minimum_required(VERSION 3.21)
project(SABRmetrics_CXX CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER /usr/bin/g++-11)
set(CMAKE_CXX_FLAGS "-Wall -pedantic -pthread -std=c++17 -fmodules-ts")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
include_directories(
        /opt/boost/boost
)
add_executable(SABRmetrics_CXX main.cpp)
target_link_libraries(SABRmetrics_CXX boost_system boost_filesystem boost_context)
