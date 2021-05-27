cmake_minimum_required(VERSION 3.18)

project(SkyMedia VERSION 1.0.0)

#find_package(LibVLC)

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
  Core Xml XmlPatterns Network Gui OpenGL Quick Widgets ${extras} #   Qml Widgets Quick Svg 
  CONFIG REQUIRED
)

#include(externals.cmake)
include(files.cmake)

set(SkyMedia_TARGET_LIBRARIES)

if(QT_DEFAULT_MAJOR_VERSION GREATER 4)
  list(APPEND SkyMedia_TARGET_LIBRARIES
      Qt5::Core Qt5::Gui Qt5::Network Qt5::OpenGL Qt5::Quick Qt5::Widgets Qt5::Xml Qt5::XmlPatterns) #  Qt5::Widgets Qt5::WidgetsPrivate Qt5::Qml Qt5::Quick Qt5::QuickPrivate Qt5::Svg
else()
  list(APPEND SkyMedia_TARGET_LIBRARIES QT_LIBRARIES)
endif()

list(APPEND SkyMedia_TARGET_LIBRARIES)

set(QT_USE_QTOPENGL TRUE)

add_library(${PROJECT_NAME} ${TYPE_OF_LIBRARY} ${CPP} ${HPP})

set(TORRENT_LINKING_SHARED ON)

target_link_libraries(${PROJECT_NAME} PRIVATE SkyCore SkyGui ${SkyMedia_TARGET_LIBRARIES} PUBLIC ${LIBVLC_LIBRARY} ${LIBVLCCORE_LIBRARY})

set(target_folders_public
  ${PROJECT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/controllers
  ${CMAKE_CURRENT_SOURCE_DIR}/include/media
  ${CMAKE_CURRENT_SOURCE_DIR}/include/vlc
  ${LIBVLC_INCLUDE_DIR}
)

set(target_folders_private
  include/3rdparty/vlc
  include/3rdparty/opengl
  include/private
  include/private/controllers
  include/private/media
  include/private/vlc
)

# Added necessary headers so that it can compile.

target_include_directories(${PROJECT_NAME} PUBLIC 
  "$<BUILD_INTERFACE:${target_folders_public}>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>"
  )

#target_include_directories(${PROJECT_NAME} PUBLIC ${target_folders_public})
#target_include_directories(${PROJECT_NAME} PUBLIC ${LIBVLC_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${target_folders_private})

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
  BASE_NAME "SkyMedia"
  EXPORT_MACRO_NAME "SKY_MEDIA_EXPORT"
  EXPORT_FILE_NAME "SkyMediaExports.h"
  )

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  )

write_basic_package_version_file(
${PROJECT_NAME}ConfigVersion.cmake
VERSION ${PACKAGE_VERSION}
COMPATIBILITY AnyNewerVersion
)

install(FILES ${HPP} DESTINATION include/${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}Targets
FILE ${PROJECT_NAME}Targets.cmake
NAMESPACE ${PROJECT_NAMESPACE}::
DESTINATION lib/CMake/
)