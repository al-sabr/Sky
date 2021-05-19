cmake_minimum_required(VERSION 3.18)

project(Sky VERSION 1.0.0)

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

export(TARGETS SkyCore SkyBackend SkyGui SkyMedia SkyTorrent SkyWeb NAMESPACE SkyFramework:: FILE SkyFrameworkTargets.cmake)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    SkyFrameworkConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )

configure_file(SkyFrameworkConfig.cmake.in SkyFrameworkConfig.cmake @ONLY)