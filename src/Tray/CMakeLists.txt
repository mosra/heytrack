find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${HeyTrack_SOURCE_DIR}/src ${HeyTrack_BINARY_DIR}/src)

set(HeyTrack_Tray_SRCS
    HeyTrack.cpp
    main.cpp
)

qt4_wrap_cpp(HeyTrack_Tray_MOC
    HeyTrack.h
)

add_executable(heytrack ${HeyTrack_Tray_SRCS} ${HeyTrack_Tray_MOC})
target_link_libraries(heytrack HeyTrackCore HeyTrackSettings ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

install(TARGETS heytrack DESTINATION bin)
