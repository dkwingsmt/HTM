cmake_minimum_required(VERSION 2.6)

project(htm07)

SET ( CMAKE_BUILD_TYPE "Debug" )
#SET ( CMAKE_BUILD_TYPE "Release" )

if( CMAKE_BUILD_TYPE MATCHES "Debug" )
    add_definitions("-DDEBUG -g")
elseif( CMAKE_BUILD_TYPE MATCHES "Release" )
    add_definitions("-DRELEASE")
endif()

add_subdirectory(core)

