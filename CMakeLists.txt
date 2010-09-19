project(HeyTrack)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules/")

# Paths
if(WIN32)
    set(HEYTRACK_BINARY_DIR .)
    set(HEYTRACK_LIBRARY_DIR .)
    set(HEYTRACK_TRANSLATION_DIR ./l10n)
else()
    set(HEYTRACK_BINARY_DIR ${CMAKE_INSTALL_PREFIX}/bin)
    set(HEYTRACK_LIBRARY_DIR ${CMAKE_INSTALL_PREFIX}/lib)
    set(HEYTRACK_TRANSLATION_DIR ${CMAKE_INSTALL_PREFIX}/share/heytrack/l10n)
endif()

add_subdirectory(src)
add_subdirectory(l10n)
