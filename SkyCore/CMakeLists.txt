cmake_minimum_required(VERSION 3.18)

project(SkyCore VERSION 1.0.0)

#
# 3rd party dependencies
#
find_package(Qt5 COMPONENTS
  Core Network Gui Widgets Qml Xml XmlPatterns #Script ScriptTools
  #Network Widgets  
  REQUIRED
)

set(SK_NOSCRIPT TRUE)

include(files.cmake)

set(SkyCore_TARGET_LIBRARIES)

if(QT_DEFAULT_MAJOR_VERSION GREATER 4)
  list(APPEND SkyCore_TARGET_LIBRARIES
    Qt5::Core Qt5::Network Qt5::Gui Qt5::Widgets Qt5::Qml Qt5::Xml Qt5::XmlPatterns #Qt5::Script Qt5::ScriptTools
    )
else()
  list(APPEND SkyCore_TARGET_LIBRARIES QT_LIBRARIES)
endif()

list(APPEND SkyCore_TARGET_LIBRARIES QuaZip)

add_library(${PROJECT_NAME} ${TYPE_OF_LIBRARY} ${CPP} ${HPP})
add_library(${PROJECT_NAMESPACE}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC ${SkyCore_TARGET_LIBRARIES})

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
  #${PROJECT_BINARY_DIR}
  include
  include/private
  include/private/controllers
  include/private/io
  include/private/kernel
  include/private/network
  include/private/plugin
  include/private/thread
  
)

#include_directories(${target_folders_public} ${target_folders_private})

target_include_directories(${PROJECT_NAME} PUBLIC ${target_folders_public})
target_include_directories(${PROJECT_NAME} PRIVATE ${target_folders_private})

add_definitions(-DQT_${QT_DEFAULT_MAJOR_VERSION} -DQT_LATEST -DSK_NO_SCRIPT) #-DQT_QTLOCKEDFILE_IMPORT

# -DSK_NO_UNZIPPER -DSK_NO_CONTROLLERFILE -DSK_NO_CACHE -DSK_NO_FILEWATCHER -DSK_NO_ABSTRACTLOADER -DSK_NO_LOADERNETWORK -DSK_NO_CONTROLLERDOWNLOAD -DSK_NO_CONTROLLERPLUGIN  -DSK_NO_YAMLREADER -DSK_NO_PLUGIN -DSK_NO_COOKIEJAR

if(WIN32)
  add_definitions(-DSKY_WIN_NATIVE -D_UNICODE -DUNICODE)
  target_compile_definitions (${PROJECT_NAME} PRIVATE -D_UNICODE -DUNICODE)
  set_target_properties(${PROJECT_NAME} PROPERTIES VS_GLOBAL_VcpkgEnabled true)
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
  BASE_NAME "SkyCore"
  EXPORT_MACRO_NAME "SKY_CORE_EXPORT"
  EXPORT_FILE_NAME "SkyCoreExports.h"
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

