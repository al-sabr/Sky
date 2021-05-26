cmake_minimum_required(VERSION 3.18)

project(SkyWeb VERSION 1.0.0)

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
  WebEngineWidgets ${extras} #Core Gui Qml Widgets 
  CONFIG REQUIRED
)

include(files.cmake)

set(SkyWeb_TARGET_LIBRARIES)

if(QT_DEFAULT_MAJOR_VERSION GREATER 4)
  list(APPEND SkyWeb_TARGET_LIBRARIES
      Qt5::WebEngineWidgets) #Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Qml 
else()
  list(APPEND SkyWeb_TARGET_LIBRARIES QT_LIBRARIES)
endif()

list(APPEND SkyWeb_TARGET_LIBRARIES)

add_library(${PROJECT_NAME} ${TYPE_OF_LIBRARY} ${CPP} ${HPP})

target_link_libraries(${PROJECT_NAME} PRIVATE ${SkyWeb_TARGET_LIBRARIES} SkyCore)

set(target_folders_public
  ${PROJECT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/io
)

set(target_folders_private
  include/private
  include/private/io
)

#include_directories(${target_folders_public} ${target_folders_private})

target_include_directories(${PROJECT_NAME} PUBLIC 
  "$<BUILD_INTERFACE:${target_folders_public}>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>"
  )

target_include_directories(${PROJECT_NAME} PRIVATE ${target_folders_private})

add_definitions(-DQT_${QT_DEFAULT_MAJOR_VERSION} -DQT_LATEST -DSK_WEB_LIBRARY -DSK_DESKTOP) #-DQT_QTLOCKEDFILE_IMPORT -DSKY_CORE_IMPORT -DTORRENT_LINKING_SHARED

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
  BASE_NAME "SkyWeb"
  EXPORT_MACRO_NAME "SKY_WEB_EXPORT"
  EXPORT_FILE_NAME "SkyWebExports.h"
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