cmake_minimum_required(VERSION 2.6)

project(HTM07)

SET ( CMAKE_BUILD_TYPE "Debug" )
#SET ( CMAKE_BUILD_TYPE "Release" )

add_definitions("-O0")

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -DDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -DRELEASE")

include_directories(${HTM07_SOURCE_DIR}/core)

link_directories(${HTM07_BINARY_DIR}/core)

add_subdirectory(core)
add_subdirectory(test)
add_subdirectory(utility)

