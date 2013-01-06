find_package(OpenCV REQUIRED)

set(cvBlob_CVBLOB  cvblob.cpp
                   cvlabel.cpp
                   cvaux.cpp
                   cvcontour.cpp
                   cvtrack.cpp
                   cvcolor.cpp
                   cvRender.cpp
)

set_source_files_properties(${cvBlob_SRC}
                            PROPERTIES
                            COMPILE_FLAGS "-O3"
)

add_library(cvblob SHARED ${cvBlob_CVBLOB})

target_link_libraries(cvblob ${OpenCV_LIBS})

install(FILES cvblob.h cvRender.h DESTINATION include/cvblob)

install(TARGETS cvblob
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)
