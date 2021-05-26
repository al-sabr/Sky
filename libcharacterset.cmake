include(FetchContent)

FetchContent_Declare(
        libcharsetdetect
        GIT_REPOSITORY "https://github.com/al-sabr/libcharsetdetect"
)

FetchContent_GetProperties(libcharsetdetect) #mispelled name in original post
if(NOT libcharsetdetect_POPULATED)   # name is lowercased
  FetchContent_Populate(libcharsetdetect)
  message(STATUS "libcharsetdetect source dir: ${libcharsetdetect_SOURCE_DIR}")
  message(STATUS "libcharsetdetect binary dir: ${libcharsetdetect_BINARY_DIR}")
  add_subdirectory(${libcharsetdetect_SOURCE_DIR} ${libcharsetdetect_BINARY_DIR}) # name is lowercased
endif()

FetchContent_MakeAvailable(libcharsetdetect)