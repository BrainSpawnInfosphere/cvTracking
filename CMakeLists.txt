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
#add_subdirectory(test)
add_subdirectory(samples)

#--- Package Config -------------------------------------------------
#set(CMAKE_INCLUDE_DIRS_CONFIGCMAKE "${CMAKE_INSTALL_PREFIX}/include/")
#set(CMAKE_LIB_DIRS_CONFIGCMAKE "${CMAKE_INSTALL_PREFIX}/lib")
#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cvtracking.pc.cmake.in" "${CMAKE_BINARY_DIR}/cvtracking.pc" @ONLY IMMEDIATE)
#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cvTrackingConfig.cmake.in" "${CMAKE_BINARY_DIR}/cvTrackingConfig.cmake" IMMEDIATE @ONLY)

#--- Install ----------------------------------------------------------
#install(FILES "${CMAKE_BINARY_DIR}/cvtracking.pc" 
#        DESTINATION lib/pkgconfig
#        )
#install(FILES ${CMAKE_BINARY_DIR}/cvTrackingConfig.cmake 
#        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cvtracking/
#        )
