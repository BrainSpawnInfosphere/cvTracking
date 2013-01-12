find_package(OpenCV REQUIRED)

include_directories(${cvTracking_SOURCE_DIR}/cvTracking)

# TEST
add_executable(test test.cpp)
target_link_libraries(test cvtracking ${OpenCV_LIBS})

# TEST_RANDOM
#add_executable(test_random test_random.cpp)
#target_link_libraries(test_random cvtracking ${OpenCV_LIBS})

# TEST TRACKING
add_executable(test_tracking test_tracking.cpp)
target_link_libraries(test_tracking cvtracking ${OpenCV_LIBS})
