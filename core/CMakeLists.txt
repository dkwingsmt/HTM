cmake_minimum_required(VERSION 2.6)

project(htm07)

set( CORE_SOURCE_FILES
    SpatialPooler.cpp
    Node.cpp
    Layer.cpp
    Space.cpp
    Common.cpp
    Introspection.cpp
    TemporalPooler.cpp
    )

add_library( htm07core ${CORE_SOURCE_FILES} )
