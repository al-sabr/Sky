include(FetchContent)

FetchContent_Declare(
        libvlcpp
        GIT_REPOSITORY "https://github.com/al-sabr/libvlcpp"
)

FetchContent_GetProperties(libvlcpp) #mispelled name in original post
if(NOT libvlcpp_POPULATED)   # name is lowercased
  FetchContent_Populate(libvlcpp)
  message(STATUS "libvlcpp source dir: ${libvlcpp_SOURCE_DIR}")
  message(STATUS "libvlcpp binary dir: ${libvlcpp_BINARY_DIR}")
  add_subdirectory(${libvlcpp_SOURCE_DIR} ${libvlcpp_BINARY_DIR}) # name is lowercased
endif()

FetchContent_MakeAvailable(libvlcpp)