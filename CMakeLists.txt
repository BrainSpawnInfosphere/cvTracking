cmake_minimum_required(VERSION 2.8)

project(cvBlob)

set(CVBLOB_VERSION_MAJOR 0)
set(CVBLOB_VERSION_MINOR 11)
set(CVBLOB_VERSION_PATCH 0)
set(CVBLOB_VERSION "${CVBLOB_VERSION_MAJOR}.${CVBLOB_VERSION_MINOR}.${CVBLOB_VERSION_PATCH}")

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

set(prefix  ${CMAKE_INSTALL_PREFIX})
set(version ${CVBLOB_VERSION})

#--- Code -----------------------------------------------------------
add_subdirectory(cvBlob)
add_subdirectory(test)
#add_subdirectory(samples)

#--- Package Config -------------------------------------------------
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cvblob.pc.cmake.in" "${CMAKE_BINARY_DIR}/cvblob.pc" @ONLY IMMEDIATE)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cvBlobConfig.cmake.in" "${CMAKE_BINARY_DIR}/cvBlobConfig.cmake" IMMEDIATE @ONLY)

#--- Documentation --------------------------------------------------
# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc 
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

#--- Install ----------------------------------------------------------
install(FILES "${CMAKE_BINARY_DIR}/cvblob.pc" 
        DESTINATION lib/pkgconfig
        )
install(FILES ${CMAKE_BINARY_DIR}/cvBlobConfig.cmake 
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cvblob/
        )
