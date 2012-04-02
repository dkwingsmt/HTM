cmake_minimum_required(VERSION 2.6)

project(htm07)

set( CORE_SOURCE_FILES
    main.cpp
    SpatialPooler.cpp
    Node.cpp
    Layer.cpp
    Space.cpp
    Common.cpp
    )

add_executable( htm07 ${CORE_SOURCE_FILES} )
