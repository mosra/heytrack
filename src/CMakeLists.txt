configure_file(configure.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/configure.h)

add_definitions("-fvisibility=default")

add_subdirectory(Core)
add_subdirectory(Settings)
add_subdirectory(Tray)

find_package(KDE4)

if(KDE4_FOUND)
    add_subdirectory(Plasmoid)
endif(KDE4_FOUND)
