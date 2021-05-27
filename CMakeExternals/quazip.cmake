include(FetchContent)

FetchContent_Declare(
        quazip
        GIT_REPOSITORY "https://github.com/al-sabr/quazip"
)

FetchContent_GetProperties(quazip) #mispelled name in original post
if(NOT quazip_POPULATED)   # name is lowercased
  FetchContent_Populate(quazip)
  message(STATUS "quazip source dir: ${quazip_SOURCE_DIR}")
  message(STATUS "quazip binary dir: ${quazip_BINARY_DIR}")
  add_subdirectory(${quazip_SOURCE_DIR} ${quazip_BINARY_DIR}) # name is lowercased
endif()

FetchContent_MakeAvailable(quazip)