cmake_minimum_required(VERSION 3.21)
project(SABRmetrics_CXX
        VERSION 0.0.1
        DESCRIPTION "SABR metrics"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER /usr/bin/g++-11)
set(CMAKE_CXX_FLAGS "-Wall -pedantic -pthread -std=c++20 -fmodules-ts")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
include_directories(
        /opt/boost/boost
        include
)
link_directories(
        /opt/boost/boost/stage/lib
)

add_executable(SABRmetrics_CXX
        main.cpp
        include/functional_tools.h
        src/Spahn.cpp
        src/Spahn.h src/DataTable.cpp src/DataTable.h src/Baseball.cpp src/Baseball.h src/Teams.cpp src/Teams.h)
target_link_libraries(SABRmetrics_CXX boost_system boost_filesystem boost_context)
