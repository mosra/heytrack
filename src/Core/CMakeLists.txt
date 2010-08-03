find_package(Qt4 REQUIRED)
find_package(QJSON REQUIRED)

include_directories(${QJSON_INCLUDE_DIRS})

set(HeyTrack_Core_SRCS
    AbstractServer.cpp
    AbRadioServer.cpp
    AbstractPlayer.cpp
    VlcPlayer.cpp
    StationModel.cpp
    FormatModel.cpp
    GenreModel.cpp
)

qt4_wrap_cpp(HeyTrack_Core_MOC
    AbstractServer.h
    AbstractPlayer.h
    StationModel.h
    FormatModel.h
    GenreModel.h
)

add_library(HeyTrackCore SHARED ${HeyTrack_Core_SRCS} ${HeyTrack_Core_MOC})
target_link_libraries(HeyTrackCore ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QJSON_LIBRARIES} ${QT_QTXMLPATTERNS_LIBRARY})

install(TARGETS HeyTrackCore DESTINATION lib)
