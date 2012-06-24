find_package(OpenCV REQUIRED)
add_executable( image_sequence image_sequence.cpp )
add_executable( image_sequence_extended image_sequence_extended.cpp)
target_link_libraries( image_sequence ${OpenCV_LIBS})
target_link_libraries( image_sequence_extended ${OpenCV_LIBS})

