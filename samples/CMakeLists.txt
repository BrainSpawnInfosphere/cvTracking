include_directories(${cvBlob_SOURCE_DIR}/cvBlob)

add_executable(red_object red_object_tracking.cpp)

target_link_libraries(red_object cvblob)
target_link_libraries(red_object ${OpenCV_LIBS})
