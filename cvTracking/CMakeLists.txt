find_package(OpenCV REQUIRED)

set(cvTracking_SRC  cvblob.cpp
                   cvtrack.cpp
                   cvRender.cpp
)

#set_source_files_properties(${cvTracking_SRC}
#                            PROPERTIES
#                            COMPILE_FLAGS "-O3"
#)

add_library(cvtracking SHARED ${cvTracking_SRC})

target_link_libraries(cvtracking ${OpenCV_LIBS})

install(FILES cvt.h cvRender.h DESTINATION include/cvtracking)

install(TARGETS cvtracking
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)
