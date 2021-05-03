cmake_minimum_required(VERSION 3.18)

project(SkyTorrent)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(WIN32)
  set(extras WinExtras)
endif()
#
# 3rd party dependencies
#
find_package(Qt5 COMPONENTS
  Core Quick Network ${extras}
  REQUIRED
)

include(files.cmake)
include(externals.cmake)

set(SkyTorrent_TARGET_LIBRARIES)

if(QT_DEFAULT_MAJOR_VERSION GREATER 4)
  message("QT_DEFAULT_MAJOR_VERSION = ${QT_DEFAULT_MAJOR_VERSION}")
  list(APPEND SkyTorrent_TARGET_LIBRARIES
    Qt5::Core Qt5::Network Qt5::Quick Qt5::QuickPrivate)
  message("SkyTorrent_TARGET_LIBRARIES = ${SkyTorrent_TARGET_LIBRARIES}")
else()
  list(APPEND SkyTorrent_TARGET_LIBRARIES QT_LIBRARIES)
endif()

list(APPEND SkyTorrent_TARGET_LIBRARIES)

set(QT_USE_QTOPENGL TRUE)
set(BUILD_SHARED_LIBS ON)

if(NOT DEFINED BUILD_SHARED_LIBS)
  set(TYPE_OF_LIBRARY "STATIC")
else()
  set(TYPE_OF_LIBRARY "SHARED")
endif()

message("")
message("CPP = ${CPP}")
message("")
message("HPP = ${HPP}")
message("")

add_library(${PROJECT_NAME} ${TYPE_OF_LIBRARY} ${CPP} ${HPP})

set(TORRENT_LINKING_SHARED ON)

target_link_libraries(${PROJECT_NAME} ${SkyTorrent_TARGET_LIBRARIES} SkyCore SkyGui torrent-rasterbar) #LibtorrentRasterbar::torrent-rasterbar

set(target_folders_public
  ${PROJECT_BINARY_DIR}
  include
  #include/libtorrent
  include/controllers
  include/io
  include/media
  include/torrent
)

set(target_folders_private
  include/private
  include/private/controllers
  include/private/io
  include/private/media
  include/private/torrent
)

include_directories(${target_folders_public} ${target_folders_private})

target_include_directories(${PROJECT_NAME} PUBLIC ${target_folders_public})
target_include_directories(${PROJECT_NAME} PUBLIC ${target_folders_private})

add_definitions(-DQT_${QT_DEFAULT_MAJOR_VERSION} -DQT_LATEST -DSK_TORRENT_LIBRARY -DSK_DESKTOP) #-DQT_QTLOCKEDFILE_IMPORT -DSKY_CORE_IMPORT -DTORRENT_LINKING_SHARED

if(WIN32 AND MSVC)
  add_definitions(-DWIN32_LEAN_AND_MEAN -DBOOST_ALL_NO_LIB -DSK_WIN_NATIVE) #
  target_compile_definitions (${PROJECT_NAME} PRIVATE -DWIN32_LEAN_AND_MEAN -DBOOST_ALL_NO_LIB -DSK_WIN_NATIVE)
  set_target_properties(${PROJECT_NAME} PROPERTIES VS_GLOBAL_VcpkgEnabled true)
endif()

# libtorrent definitions
add_definitions(-DTORRENT_LINKING_SHARED)

# -DSK_NO_UNZIPPER -DSK_NO_CONTROLLERFILE -DSK_NO_CACHE -DSK_NO_FILEWATCHER -DSK_NO_ABSTRACTLOADER -DSK_NO_LOADERNETWORK -DSK_NO_CONTROLLERDOWNLOAD -DSK_NO_CONTROLLERPLUGIN  -DSK_NO_YAMLREADER -DSK_NO_PLUGIN -DSK_NO_COOKIEJAR

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
  BASE_NAME "SkyTorrent"
  EXPORT_MACRO_NAME "SKY_TORRENT_EXPORT"
  EXPORT_FILE_NAME "SkyTorrentExports.h"
  )