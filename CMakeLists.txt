cmake_minimum_required(VERSION 3.18)

project(Sky VERSION 1.0.0)

if(NOT DEFINED BUILD_SHARED_LIBS)
  set(TYPE_OF_LIBRARY "STATIC")
else()
  set(TYPE_OF_LIBRARY "SHARED")
endif()

set(LIBVLC_LIBRARY "" CACHE FILEPATH "Path of libVLC")
set(LIBVLCCORE_LIBRARY "" CACHE FILEPATH "Path of libVLC Core")
set(LIBVLC_INCLUDE_DIR "" CACHE PATH "Path of libVLC Includes")

if(LIBVLC_LIBRARY AND LIBVLCCORE_LIBRARY)
  include(externals.cmake)
endif()

#add_subdirectory(vlc)
add_subdirectory(libtorrent)
add_subdirectory(SkyCore)
add_subdirectory(SkyBackend)
add_subdirectory(SkyGui)
add_subdirectory(SkyMedia)
add_subdirectory(SkyTorrent)
add_subdirectory(SkyWeb)

#include(downloads.cmake)



#set(LIBVLC_INCLUDE_DIR ${SkyMedia_DIR}/include/vlc)
#set(LIBVLC_LIBRARY "${vlc_LOCATION}/libvlc.lib")

export(TARGETS SkyCore SkyBackend SkyGui SkyMedia SkyTorrent SkyWeb QuaZip torrent-rasterbar NAMESPACE SkyFramework:: FILE SkyFrameworkTargets.cmake)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    SkyFrameworkConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )

configure_file(SkyFrameworkConfig.cmake.in SkyFrameworkConfig.cmake @ONLY)