cmake_minimum_required(VERSION 3.18)

project(SkyGui VERSION 1.0.0)

if(WIN32)
  set(extras WinExtras)
endif()
#
# 3rd party dependencies
#
find_package(Qt5 COMPONENTS
  Core Quick Svg Xml XmlPatterns ${extras} # Gui Widgets Qml Widgets   OpenGL
  REQUIRED
)

include(files.cmake)

set(SkyGui_TARGET_LIBRARIES)

if(QT_DEFAULT_MAJOR_VERSION GREATER 4)
  list(APPEND SkyGui_TARGET_LIBRARIES
    Qt5::Core Qt5::Quick Qt5::QuickPrivate Qt5::Svg Qt5::Xml Qt5::XmlPatterns) # Qt5::Gui Qt5::Widgets Qt5::WidgetsPrivate Qt5::Qml Qt5::Quick Qt5::QuickPrivate 
else()
  list(APPEND SkyGui_TARGET_LIBRARIES QT_LIBRARIES)
endif()

if(WIN32)
  list(APPEND SkyGui_TARGET_LIBRARIES Qt5::WinExtras Qt5::WinExtrasPrivate)
endif()

set(QT_USE_QTOPENGL TRUE)

add_library(${PROJECT_NAME} ${TYPE_OF_LIBRARY} ${CPP} ${HPP})

target_link_libraries(${PROJECT_NAME} PRIVATE SkyCore ${SkyGui_TARGET_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES VS_GLOBAL_VcpkgEnabled true)

set(target_folders_public
  ${PROJECT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/3rdparty/qtsingleapplication/
  ${CMAKE_CURRENT_SOURCE_DIR}/include/controllers
  ${CMAKE_CURRENT_SOURCE_DIR}/include/declarative
  ${CMAKE_CURRENT_SOURCE_DIR}/include/graphicsview
  ${CMAKE_CURRENT_SOURCE_DIR}/include/image
  ${CMAKE_CURRENT_SOURCE_DIR}/include/io
  ${CMAKE_CURRENT_SOURCE_DIR}/include/kernel
  ${CMAKE_CURRENT_SOURCE_DIR}/include/media
  ${CMAKE_CURRENT_SOURCE_DIR}/include/models
)

set(target_folders_private
  include/private
  include/private/controllers
  include/private/declarative
  include/private/graphicsview
  include/private/image
  include/private/io
  include/private/kernel
  include/private/media
  include/private/models
)

#include_directories(${target_folders_public} ${target_folders_private} ${SkyGui_INCLUDE_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC 
  "$<BUILD_INTERFACE:${target_folders_public}>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>"
  )

target_include_directories(${PROJECT_NAME} PRIVATE ${target_folders_private})

add_definitions(-DQT_${QT_DEFAULT_MAJOR_VERSION} -DQT_LATEST -DQT_QTLOCKEDFILE_IMPORT) #-DQT_QTLOCKEDFILE_IMPORT

if(WIN32)
  add_definitions(-DSKY_WIN_NATIVE -D_UNICODE -DUNICODE)
  target_compile_definitions (${PROJECT_NAME} PRIVATE -D_UNICODE -DUNICODE)
endif()

# -DSK_NO_UNZIPPER -DSK_NO_CONTROLLERFILE -DSK_NO_CACHE -DSK_NO_FILEWATCHER -DSK_NO_ABSTRACTLOADER -DSK_NO_LOADERNETWORK -DSK_NO_CONTROLLERDOWNLOAD -DSK_NO_CONTROLLERPLUGIN  -DSK_NO_YAMLREADER -DSK_NO_PLUGIN -DSK_NO_COOKIEJAR

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
  BASE_NAME "SkyGui"
  EXPORT_MACRO_NAME "SKY_GUI_EXPORT"
  EXPORT_FILE_NAME "SkyGuiExports.h"
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