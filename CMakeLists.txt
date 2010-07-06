project(HeyTrack)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules/")

find_package(Qt4 REQUIRED)
find_package(QJSON REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${QJSON_INCLUDE_DIRS})

add_subdirectory(src)
