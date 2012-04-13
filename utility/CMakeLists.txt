find_package(OpenCV REQUIRED)
add_executable( image_sequence image_sequence.cpp )
target_link_libraries( image_sequence ${OpenCV_LIBS})
