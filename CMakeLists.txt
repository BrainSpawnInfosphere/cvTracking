cmake_minimum_required(VERSION 2.8)

project(cvTracking)

set(CVTRACKING_VERSION_MAJOR 0)
set(CVTRACKING_VERSION_MINOR 1)
set(CVTRACKING_VERSION_PATCH 0)
set(CVTRACKING_VERSION "${CVTRACKING_VERSION_MAJOR}.${CVTRACKING_VERSION_MINOR}.${CVTRACKING_VERSION_PATCH}")

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

set(prefix  ${CMAKE_INSTALL_PREFIX})
set(version ${CVTRACKING_VERSION})

#--- Code -----------------------------------------------------------
add_subdirectory(cvTracking)
add_subdirectory(test)
add_subdirectory(samples)

#--- Package Config -------------------------------------------------
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cvtracking.pc.cmake.in" "${CMAKE_BINARY_DIR}/cvtracking.pc" @ONLY IMMEDIATE)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cvTrackingConfig.cmake.in" "${CMAKE_BINARY_DIR}/cvTrackingConfig.cmake" IMMEDIATE @ONLY)

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
install(FILES "${CMAKE_BINARY_DIR}/cvtracking.pc" 
        DESTINATION lib/pkgconfig
        )
install(FILES ${CMAKE_BINARY_DIR}/cvTrackingConfig.cmake 
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cvtracking/
        )
