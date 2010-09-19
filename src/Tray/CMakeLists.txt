find_package(Qt4 REQUIRED)

include_directories(${HeyTrack_SOURCE_DIR}/src ${HeyTrack_BINARY_DIR}/src)

set(HeyTrack_Tray_SRCS
    HeyTrack.cpp
    main.cpp
)

qt4_wrap_cpp(HeyTrack_Tray_MOC
    HeyTrack.h
)

# Diable console window, executable icon for WIN32
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows -Wl,-subsystem,windows")

    enable_language(RC)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff <FLAGS> -o <OBJECT> -i <SOURCE>")
    set(HeyTrack_Tray_SRCS ${HeyTrack_Tray_SRCS} ${CMAKE_SOURCE_DIR}/graphics/win-icon.rc)
endif(WIN32)

add_executable(heytrack ${HeyTrack_Tray_SRCS} ${HeyTrack_Tray_MOC})
target_link_libraries(heytrack HeyTrackCore HeyTrackSettings ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

install(TARGETS heytrack DESTINATION ${HEYTRACK_BINARY_DIR})

# Install also runtime libs for WIN32
if(WIN32)
    include(HeyTrackMacros)

    heytrack_bundle_dlls(${HEYTRACK_LIBRARY_DIR}
        libgcc_s_dw2-1.dll
        libgcc_s_sjlj-1.dll
        libqjson.dll
        libstdc++-6.dll
        mingwm10.dll
        phonon_backend/phonon_ds94.dll
        phonon4.dll
        QtCore4.dll
        QtGui4.dll
        QtNetwork4.dll
        QtXmlPatterns4.dll
        PATHS ${DLL_SEARCH_PATH}
    )
endif()
