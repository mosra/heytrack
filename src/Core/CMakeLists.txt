find_package(Qt4 REQUIRED)
find_package(QJSON REQUIRED)

include_directories(${QJSON_INCLUDE_DIRS} ${QT_PHONON_INCLUDE_DIR})

set(HeyTrack_Core_SRCS
    AbstractServer.cpp
    AbRadioServer.cpp
    RockmaxServer.cpp
    AbstractPlayer.cpp
    VlcPlayer.cpp
    PhononPlayer.cpp
    StationModel.cpp
    FormatModel.cpp
    GenreModel.cpp
    Style.cpp
)

qt4_wrap_cpp(HeyTrack_Core_MOC
    AbstractServer.h
    AbRadioServer.h
    RockmaxServer.h
    AbstractPlayer.h
    StationModel.h
    FormatModel.h
    GenreModel.h
)

qt4_add_resources(HeyTrack_Core_QRC
    graphics/graphics.qrc
)

add_library(HeyTrackCore SHARED ${HeyTrack_Core_SRCS} ${HeyTrack_Core_MOC} ${HeyTrack_Core_QRC})
target_link_libraries(HeyTrackCore
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTXMLPATTERNS_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    ${QJSON_LIBRARIES}
    ${QT_PHONON_LIBRARY}
)

install(TARGETS HeyTrackCore DESTINATION lib)
