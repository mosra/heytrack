project(HeyTrack)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(HeyTrackSRCS HeyTrack.cpp main.cpp)
qt4_wrap_cpp(HeyTrackMOC HeyTrack.h)
add_executable(HeyTrack ${HeyTrackSRCS} ${HeyTrackMOC})
target_link_libraries(HeyTrack ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY})
