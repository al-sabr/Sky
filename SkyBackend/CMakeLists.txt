cmake_minimum_required(VERSION 3.18)

project(SkyBackend VERSION 1.0.0)

#
# 3rd party dependencies
#
find_package(Qt5 COMPONENTS
  Core Network Gui Quick
  REQUIRED
) #Gui Widgets Qml Script ScriptTools Network Widgets Xml XmlPatterns
set(SK_NOSCRIPT TRUE)

include(files.cmake)

set(SkyBackend_TARGET_LIBRARIES)

if(QT_DEFAULT_MAJOR_VERSION GREATER 4)
  list(APPEND SkyBackend_TARGET_LIBRARIES
    Qt5::Core Qt5::Network Qt5::Gui Qt5::Quick)#Qt5::Gui Qt5::Script Qt5::ScriptTools Qt5::Network Qt5::Widgets Qt5::Qml Qt5::Xml Qt5::XmlPatterns
else()
  list(APPEND SkyBackend_TARGET_LIBRARIES QT_LIBRARIES)
endif()

add_library(${PROJECT_NAME} ${TYPE_OF_LIBRARY} ${CPP} ${HPP})

target_link_libraries(${PROJECT_NAME} PRIVATE SkyCore SkyGui SkyTorrent ${SkyBackend_TARGET_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES VS_GLOBAL_VcpkgEnabled true)

set(includes_public
  ${PROJECT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/io
  ${CMAKE_CURRENT_SOURCE_DIR}/include/media
  ${CMAKE_CURRENT_SOURCE_DIR}/include/private
  ${CMAKE_CURRENT_SOURCE_DIR}/include/private/io
  ${CMAKE_CURRENT_SOURCE_DIR}/include/private/media
)
include_directories(
  ${includes_public}
  )

target_include_directories(${PROJECT_NAME} PUBLIC 
  "$<BUILD_INTERFACE:${target_folders_public}>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>"
)

add_definitions(-DQT_${QT_DEFAULT_MAJOR_VERSION} -DQT_LATEST -DSK_NO_SCRIPT)

# -DSK_NO_UNZIPPER -DSK_NO_CONTROLLERFILE -DSK_NO_CACHE -DSK_NO_FILEWATCHER -DSK_NO_ABSTRACTLOADER -DSK_NO_LOADERNETWORK -DSK_NO_CONTROLLERDOWNLOAD -DSK_NO_CONTROLLERPLUGIN  -DSK_NO_YAMLREADER -DSK_NO_PLUGIN -DSK_NO_COOKIEJAR

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
  BASE_NAME "SkyBackend"
  EXPORT_MACRO_NAME "SKY_BACKEND_EXPORT"
  EXPORT_FILE_NAME "SkyBackendExports.h"
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