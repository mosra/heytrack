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

#
# CPack configuration
#

# Package name and version
set(CPACK_PACKAGE_NAME "HeyTrack")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Simple utility for listening internet radio streams.")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "99")
set(CPACK_PACKAGE_VERSION_PATCH "70")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING.LESSER")

# Otherwise it will try to install into /usr
set(CPACK_SET_DESTDIR ON)

# Strip all files
set(CPACK_STRIP_FILES ON)

# Per-generator settings
set(CPACK_PROJECT_CONFIG_FILE "${CMAKE_SOURCE_DIR}/package/CPackOptions.cmake")

# Installer header image
set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/graphics\\\\nsis-header.bmp")

# Icon for entry in Add/Remove Programs
set(CPACK_NSIS_INSTALLED_ICON_NAME "heytrack.exe")

# Contact
set(CPACK_NSIS_HELP_LINK "http://mosra.cz/blog/heytrack.php")
set(CPACK_NSIS_URL_INFO_ABOUT "http://mosra.cz/blog/index-en.php")
set(CPACK_NSIS_CONTACT "mosra@centrum.cz")

# Executable and its caption for start menu entry
# - CPack insists on creating shortcut to bin/ subfolder, ugly workaround
# - http://public.kitware.com/Bug/view.php?id=7828
# - http://public.kitware.com/Bug/view.php?id=7829
# - TWO YEARS! TWO FUCKING YEARS AND STILL NOT FIXED! DAMN! BURN IN HELL!
set(CPACK_PACKAGE_EXECUTABLES "" "")
set(CPACK_NSIS_MENU_LINKS "heytrack.exe" "HeyTrack")

# Install directory
set(CPACK_PACKAGE_INSTALL_DIRECTORY "HeyTrack")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "HeyTrack")

# Finally include CPack (this must be at the end)
include(CPack)
