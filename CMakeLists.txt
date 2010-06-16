project(HeyTrack)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules/")

find_package(Qt4 REQUIRED)
find_package(QJSON REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${QJSON_INCLUDE_DIRS})

set(HeyTrack_SRCS
    HeyTrack.cpp
    HeySettings.cpp
    main.cpp)

qt4_wrap_cpp(HeyTrack_MOC
    HeyTrack.h
    HeySettings.h)

add_executable(HeyTrack ${HeyTrack_SRCS} ${HeyTrack_MOC})
target_link_libraries(HeyTrack ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QJSON_LIBRARIES} ${QT_QTXMLPATTERNS_LIBRARY})
