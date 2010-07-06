set(HeyTrack_SRCS
    HeyTrack.cpp
    HeySettings.cpp
    main.cpp)

qt4_wrap_cpp(HeyTrack_MOC
    HeyTrack.h
    HeySettings.h)

add_executable(HeyTrack ${HeyTrack_SRCS} ${HeyTrack_MOC})
target_link_libraries(HeyTrack ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QJSON_LIBRARIES} ${QT_QTXMLPATTERNS_LIBRARY})
