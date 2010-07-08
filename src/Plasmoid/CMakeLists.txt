find_package(KDE4 REQUIRED)

include_directories(${KDE4_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(HeyTrack_Plasmoid_SRCS
    HeyTrack.cpp
)

kde4_add_plugin(plasma_applet_heytrack ${HeyTrack_Plasmoid_SRCS})
target_link_libraries(plasma_applet_heytrack ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS})

install(TARGETS plasma_applet_heytrack DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-heytrack.desktop DESTINATION ${SERVICES_INSTALL_DIR})
