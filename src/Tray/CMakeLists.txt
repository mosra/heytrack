find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${HeyTrack_SOURCE_DIR}/src)

set(HeyTrack_Tray_SRCS
    HeyTrack.cpp
    main.cpp
)

qt4_wrap_cpp(HeyTrack_Tray_MOC
    HeyTrack.h
)

add_executable(HeyTrack ${HeyTrack_Tray_SRCS} ${HeyTrack_Tray_MOC})
target_link_libraries(HeyTrack HeyTrackCore HeyTrackSettings ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
