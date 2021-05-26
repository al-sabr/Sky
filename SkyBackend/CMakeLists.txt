cmake_minimum_required(VERSION 3.18)

project(SkyBackend)

#
# 3rd party dependencies
#
find_package(Qt5 COMPONENTS
  Core
  REQUIRED
) #Gui Widgets Qml Script ScriptTools Network Widgets Xml XmlPatterns
set(SK_NOSCRIPT TRUE)

include(files.cmake)

set(SkyBackend_TARGET_LIBRARIES)

if(QT_DEFAULT_MAJOR_VERSION GREATER 4)
  list(APPEND SkyBackend_TARGET_LIBRARIES
    Qt5::Core )#Qt5::Gui Qt5::Script Qt5::ScriptTools Qt5::Network Qt5::Widgets Qt5::Qml Qt5::Xml Qt5::XmlPatterns
else()
  list(APPEND SkyBackend_TARGET_LIBRARIES QT_LIBRARIES)
endif()

add_library(${PROJECT_NAME} ${TYPE_OF_LIBRARY} ${CPP} ${HPP})

target_link_libraries(${PROJECT_NAME} ${SkyBackend_TARGET_LIBRARIES} SkyCore SkyGui SkyTorrent)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_GLOBAL_VcpkgEnabled true)

set(includes_public
  ${PROJECT_BINARY_DIR}
  include
  include/io
  include/media
  include/private
  include/private/io
  include/private/media
)
include_directories(
  ${includes_public}
  )


target_include_directories(${PROJECT_NAME} PUBLIC ${includes_public})
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
