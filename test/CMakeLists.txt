find_package(OpenCV REQUIRED)

include_directories(${cvBlob_SOURCE_DIR}/cvBlob)

# TEST
add_executable(test test.cpp)
target_link_libraries(test cvblob ${OpenCV_LIBS})

# TEST_RANDOM
#add_executable(test_random test_random.cpp)
#target_link_libraries(test_random cvblob ${OpenCV_LIBS})

# TEST TRACKING
add_executable(test_tracking test_tracking.cpp)
target_link_libraries(test_tracking cvblob ${OpenCV_LIBS})
