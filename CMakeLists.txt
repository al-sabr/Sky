cmake_minimum_required(VERSION 3.18)

project(Sky)

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