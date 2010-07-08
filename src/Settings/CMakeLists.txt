find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${HeyTrack_SOURCE_DIR}/src)

set(HeyTrack_Settings_SRCS
    SettingsDialog.cpp
)

qt4_wrap_cpp(HeyTrack_Settings_MOC
    SettingsDialog.h
)

add_library(HeyTrackSettings STATIC ${HeyTrack_Settings_SRCS} ${HeyTrack_Settings_MOC})
target_link_libraries(HeyTrackSettings HeyTrackCore ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
