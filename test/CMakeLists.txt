find_package(OpenCV REQUIRED)

add_executable( testSpLearn testSpLearn.cpp )
target_link_libraries( testSpLearn htm07core ${OpenCV_LIBS})

add_executable( testTpSort testTpSort.cpp )
target_link_libraries( testTpSort htm07core ${OpenCV_LIBS})

add_executable( testCpSubSpace testCpSubSpace.cpp )
target_link_libraries( testCpSubSpace htm07core ${OpenCV_LIBS})

add_executable( testSpTableView testSpTableView.cpp )
target_link_libraries( testSpTableView htm07core ${OpenCV_LIBS})

add_executable( testConclude testConclude.cpp )
target_link_libraries( testConclude htm07core ${OpenCV_LIBS})
