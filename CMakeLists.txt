cmake_minimum_required(VERSION 3.18)

project(Sky)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT DEFINED BUILD_SHARED_LIBS)
  set(TYPE_OF_LIBRARY "STATIC")
else()
  set(TYPE_OF_LIBRARY "SHARED")
endif()
  
add_subdirectory(SkyCore)
add_subdirectory(SkyBackend)
add_subdirectory(SkyGui)