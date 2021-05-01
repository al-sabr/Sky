cmake_minimum_required(VERSION 3.18)

project(SkyCore)

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

set(SkyCore_TARGET_LIBRARIES)

if(QT_DEFAULT_MAJOR_VERSION GREATER 4)
  message("QT_DEFAULT_MAJOR_VERSION = ${QT_DEFAULT_MAJOR_VERSION}")
  list(APPEND SkyCore_TARGET_LIBRARIES
    Qt5::Core Qt5::Gui Qt5::Script Qt5::ScriptTools Qt5::Network Qt5::Widgets Qt5::Qml
    Qt5::Xml Qt5::XmlPatterns)
  message("SkyCore_TARGET_LIBRARIES = ${SkyCore_TARGET_LIBRARIES}")
else()
  list(APPEND SkyCore_TARGET_LIBRARIES QT_LIBRARIES)
endif()

list(APPEND SkyCore_TARGET_LIBRARIES QuaZip)

if(NOT DEFINED BUILD_SHARED_LIBS)
  set(TYPE_OF_LIBRARY "STATIC")
else()
  set(TYPE_OF_LIBRARY "SHARED")
endif()

add_library(${PROJECT_NAME} SHARED ${CPP} ${HPP})

target_link_libraries(${PROJECT_NAME} ${SkyCore_TARGET_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES VS_GLOBAL_VcpkgEnabled true)

set(target_folders_public
  ${PROJECT_BINARY_DIR}
  include
  include/3rdparty/qtsingleapplication
  include/controllers
  include/global
  include/io
  include/kernel
  include/network
  include/plugin
  include/thread
  
)
set(target_folders_private
  ${PROJECT_BINARY_DIR}
  include
  include/private
  include/private/controllers
  include/private/io
  include/private/kernel
  include/private/network
  include/private/plugin
  include/private/thread
  
)

include_directories(${target_folders_public} ${target_folders_private})

target_include_directories(${PROJECT_NAME} PUBLIC ${target_folders_public})
target_include_directories(${PROJECT_NAME} PUBLIC ${target_folders_private})

#set_property(SOURCE WControllerFile.moc PROPERTY SKIP_AUTOMOC ON)
#set_property(SOURCE src/controllers/WControllerFile.cpp PROPERTY SKIP_AUTOGEN ON)

#target_include_directories(${PROJECT_NAME} PUBLIC include)
#target_include_directories(${PROJECT_NAME} PRIVATE include/private)

add_definitions(-DQT_${QT_DEFAULT_MAJOR_VERSION} -DQT_LATEST -DSK_NO_SCRIPT) #-DQT_QTLOCKEDFILE_IMPORT

# -DSK_NO_UNZIPPER -DSK_NO_CONTROLLERFILE -DSK_NO_CACHE -DSK_NO_FILEWATCHER -DSK_NO_ABSTRACTLOADER -DSK_NO_LOADERNETWORK -DSK_NO_CONTROLLERDOWNLOAD -DSK_NO_CONTROLLERPLUGIN  -DSK_NO_YAMLREADER -DSK_NO_PLUGIN -DSK_NO_COOKIEJAR

if(WIN32)
  add_definitions(-DSKY_WIN_NATIVE -D_UNICODE -DUNICODE)
  target_compile_definitions (${PROJECT_NAME} PRIVATE -D_UNICODE -DUNICODE)
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
  BASE_NAME "SkyCore"
  EXPORT_MACRO_NAME "SKY_CORE_EXPORT"
  EXPORT_FILE_NAME "SkyCoreExports.h"
  )
  #[[
install(
  TARGETS
    ${PROJECT_NAME}
  ARCHIVE
    DESTINATION ${INSTALL_LIBDIR}
    COMPONENT lib
  RUNTIME
    DESTINATION ${INSTALL_BINDIR}
    COMPONENT bin
  LIBRARY
    DESTINATION ${INSTALL_LIBDIR}
    COMPONENT lib
  PUBLIC_HEADER
    DESTINATION ${INSTALL_INCLUDEDIR}/src
    COMPONENT dev
  )
  ]]
  
add_subdirectory(externals)