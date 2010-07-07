configure_file(configure.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/configure.h)

add_subdirectory(Core)
add_subdirectory(Settings)
add_subdirectory(Tray)
