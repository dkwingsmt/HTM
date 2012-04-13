cmake_minimum_required(VERSION 2.6)

project(HTM07)

SET ( CMAKE_BUILD_TYPE "Debug" )
#SET ( CMAKE_BUILD_TYPE "Release" )

if( CMAKE_BUILD_TYPE MATCHES "Debug" )
    add_definitions("-DDEBUG -g")
elseif( CMAKE_BUILD_TYPE MATCHES "Release" )
    add_definitions("-DRELEASE")
endif()

include_directories(${HTM07_SOURCE_DIR}/core)

link_directories(${HTM07_BINARY_DIR}/core)

add_subdirectory(core)
add_subdirectory(test)

