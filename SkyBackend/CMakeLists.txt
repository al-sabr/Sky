cmake_minimum_required(VERSION 3.18)

project(SkyBackend)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#
# 3rd party dependencies
#
find_package(Qt5 COMPONENTS
  Core Gui Widgets Qml Script ScriptTools
  Network Widgets Xml XmlPatterns
  REQUIRED
)
set(SK_NOSCRIPT TRUE)

include(files.cmake)

set(SkyBackend_TARGET_LIBRARIES)

if(QT_DEFAULT_MAJOR_VERSION GREATER 4)
  message("QT_DEFAULT_MAJOR_VERSION = ${QT_DEFAULT_MAJOR_VERSION}")
  list(APPEND SkyBackend_TARGET_LIBRARIES
    Qt5::Core Qt5::Gui Qt5::Script Qt5::ScriptTools Qt5::Network Qt5::Widgets Qt5::Qml
    Qt5::Xml Qt5::XmlPatterns)
  message("SkyBackend_TARGET_LIBRARIES = ${SkyBackend_TARGET_LIBRARIES}")
else()
  list(APPEND SkyBackend_TARGET_LIBRARIES QT_LIBRARIES)
endif()

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

add_library(${PROJECT_NAME} SHARED ${CPP} ${HPP})

target_link_libraries(${PROJECT_NAME} ${SkyBackend_TARGET_LIBRARIES} SkyCore SkyGui SkyTorrent)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_GLOBAL_VcpkgEnabled true)

include_directories(
  ${PROJECT_BINARY_DIR}
  include
  include/io
  include/media
  include/private
  include/private/io
  include/private/media
  )

#set_property(SOURCE WControllerFile.moc PROPERTY SKIP_AUTOMOC ON)
#set_property(SOURCE src/controllers/WControllerFile.cpp PROPERTY SKIP_AUTOGEN ON)

#target_include_directories(${PROJECT_NAME} PUBLIC include)
#target_include_directories(${PROJECT_NAME} PRIVATE include/private)

add_definitions(-DQT_${QT_DEFAULT_MAJOR_VERSION} -DQT_LATEST -DSK_NO_SCRIPT)

# -DSK_NO_UNZIPPER -DSK_NO_CONTROLLERFILE -DSK_NO_CACHE -DSK_NO_FILEWATCHER -DSK_NO_ABSTRACTLOADER -DSK_NO_LOADERNETWORK -DSK_NO_CONTROLLERDOWNLOAD -DSK_NO_CONTROLLERPLUGIN  -DSK_NO_YAMLREADER -DSK_NO_PLUGIN -DSK_NO_COOKIEJAR

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
  BASE_NAME "SkyBackend"
  EXPORT_MACRO_NAME "SKY_BACKEND_EXPORT"
  EXPORT_FILE_NAME "SkyBackendExports.h"
  )
