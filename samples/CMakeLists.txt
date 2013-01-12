include_directories(${cvTracking_SOURCE_DIR}/cvTracking)

add_executable(green_object green_object_tracking.cpp)

target_link_libraries(green_object cvtracking ${OpenCV_LIBS})
