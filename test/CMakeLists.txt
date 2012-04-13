find_package(OpenCV REQUIRED)
add_executable( testSpLearn testSpLearn.cpp )
target_link_libraries( testSpLearn htm07core ${OpenCV_LIBS})
