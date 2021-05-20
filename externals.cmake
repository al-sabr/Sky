set(vlc_LOCATION ${CMAKE_SOURCE_DIR}/vlc)
set(vlc_DESTINATION ${PROJECT_BINARY_DIR})

add_library(libvlc STATIC IMPORTED)
set_property(TARGET libvlc PROPERTY
IMPORTED_LOCATION "${vlc_LOCATION}/libvlc.lib")

add_library(libvlccore STATIC IMPORTED)
set_property(TARGET libvlccore PROPERTY
IMPORTED_LOCATION "${vlc_LOCATION}/libvlccore.lib")

file(COPY ${vlc_LOCATION}/libvlc.lib
  DESTINATION ${vlc_DESTINATION}
)
file(COPY ${vlc_LOCATION}/libvlccore.lib
  DESTINATION ${vlc_DESTINATION}
)

# Mark imported library for installation using a workaround since imported
# targets do not have proper support for installation in CMakeLists
install(FILES ${vlc_LOCATION}/libvlc.lib DESTINATION ${vlc_DESTINATION})
install(FILES ${vlc_LOCATION}/libvlccore.lib DESTINATION ${vlc_DESTINATION})