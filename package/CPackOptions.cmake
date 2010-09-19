if(${CPACK_GENERATOR} STREQUAL NSIS)

    # Nice package name at least for NSIS
    set(CPACK_PACKAGE_FILE_NAME heytrack-${CPACK_PACKAGE_VERSION}-installer)

endif()
