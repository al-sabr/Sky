cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAMESPACE SkyFramework)
set(PROJECT_VERSION 1.0.0)

project(${PROJECT_NAMESPACE} VERSION ${PROJECT_VERSION})

#set(CMAKE_INSTALL_PREFIX ${PROJECT_NAMESPACE})

find_package(Qt5 COMPONENTS Core)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

set(BUILD_SHARED_LIBS ON)

if(NOT DEFINED BUILD_SHARED_LIBS)
  set(TYPE_OF_LIBRARY "STATIC")
else()
  set(TYPE_OF_LIBRARY "SHARED")
endif()

add_subdirectory(CMakeExternals)
add_subdirectory(SkyCore)
add_subdirectory(SkyBackend)
add_subdirectory(SkyGui)
add_subdirectory(SkyMedia)
add_subdirectory(SkyTorrent)
add_subdirectory(SkyWeb)

export(TARGETS SkyCore SkyBackend SkyGui SkyMedia SkyTorrent SkyWeb NAMESPACE ${PROJECT_NAMESPACE}:: FILE SkyFrameworkTargets.cmake)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    SkyFrameworkConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )

configure_file(SkyFrameworkConfig.cmake.in SkyFrameworkConfig.cmake @ONLY)

install(FILES ${LIBVLC_LIBRARY} ${LIBVLCCORE_LIBRARY} DESTINATION lib)
install(FILES ${LIBVLC_RUNTIME} ${LIBVLCCORE_RUNTIME} DESTINATION bin)