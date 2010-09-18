find_package(Qt4 REQUIRED)

include_directories(${HeyTrack_SOURCE_DIR}/src ${HeyTrack_BINARY_DIR}/src)

set(HeyTrack_Tray_SRCS
    HeyTrack.cpp
    main.cpp
)

qt4_wrap_cpp(HeyTrack_Tray_MOC
    HeyTrack.h
)

if(WIN32)
    enable_language(RC)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff <FLAGS> -o <OBJECT> -i <SOURCE>")
    set(HeyTrack_Tray_SRCS ${HeyTrack_Tray_SRCS} ${CMAKE_SOURCE_DIR}/graphics/win-icon.rc)
endif(WIN32)

add_executable(heytrack ${HeyTrack_Tray_SRCS} ${HeyTrack_Tray_MOC})
target_link_libraries(heytrack HeyTrackCore HeyTrackSettings ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

install(TARGETS heytrack DESTINATION bin)
