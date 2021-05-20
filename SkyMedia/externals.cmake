add_library(libvlc STATIC IMPORTED)
set_property(TARGET libvlc PROPERTY
IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/vlc/libvlc.lib")

add_library(libvlccore STATIC IMPORTED)
set_property(TARGET libvlccore PROPERTY
IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/vlc/libvlccore.lib")