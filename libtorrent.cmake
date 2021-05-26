include(FetchContent)

FetchContent_Declare(
        libtorrent
        GIT_REPOSITORY "https://github.com/al-sabr/libtorrent"
)

FetchContent_GetProperties(libtorrent) #mispelled name in original post
if(NOT libtorrent_POPULATED)   # name is lowercased
  FetchContent_Populate(libtorrent)
  message(STATUS "libtorrent source dir: ${libtorrent_SOURCE_DIR}")
  message(STATUS "libtorrent binary dir: ${libtorrent_BINARY_DIR}")
  add_subdirectory(${libtorrent_SOURCE_DIR} ${libtorrent_BINARY_DIR}) # name is lowercased
endif()

FetchContent_MakeAvailable(libtorrent)