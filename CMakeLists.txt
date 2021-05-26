cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAMESPACE SkyFramework)

project(${PROJECT_NAMESPACE} VERSION 1.0.0)

set(CMAKE_INSTALL_PREFIX ${PROJECT_NAMESPACE})

include(quazip.cmake)
include(libcharacterset.cmake)
include(libvlcpp.cmake)

find_package(Qt5 COMPONENTS Core)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake")

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(BUILD_SHARED_LIBS ON)

if(NOT DEFINED BUILD_SHARED_LIBS)
  set(TYPE_OF_LIBRARY "STATIC")
else()
  set(TYPE_OF_LIBRARY "SHARED")
endif()

add_subdirectory(libtorrent)
add_subdirectory(SkyCore)
add_subdirectory(SkyBackend)
add_subdirectory(SkyGui)
add_subdirectory(SkyMedia)
add_subdirectory(SkyTorrent)
add_subdirectory(SkyWeb)

export(TARGETS SkyCore SkyBackend SkyGui SkyMedia SkyTorrent SkyWeb QuaZip torrent-rasterbar NAMESPACE SkyFramework:: FILE SkyFrameworkTargets.cmake)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    SkyFrameworkConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )

configure_file(SkyFrameworkConfig.cmake.in SkyFrameworkConfig.cmake @ONLY)