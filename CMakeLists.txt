project(HeyTrack)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules/")
set(HEYTRACK_DATA_DIR ${CMAKE_INSTALL_PREFIX}/share/heytrack)

add_subdirectory(src)
add_subdirectory(l10n)
