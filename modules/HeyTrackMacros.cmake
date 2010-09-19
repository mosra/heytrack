#
# Find and install DLLs for bundling with Windows build.
#
# Usage:
#       heytrack_bundle_dlls(library_install_dir dll1 ddl2 ... PATHS path1 path2...)
#
# DLL names can also contain paths, they will be installed into exact
# specified path. If an DLL is not found, fatal error message will be printed.
#
function(heytrack_bundle_dlls library_install_dir)
    # Get DLL and path lists
    foreach(arg ${ARGN})
        if(${arg} STREQUAL PATHS)
            set(__DOING_PATHS ON)
        else()
            if(__DOING_PATHS)
                set(paths ${paths} ${arg})
            else()
                set(dlls ${dlls} ${arg})
            endif()
        endif()
    endforeach()

    # Find and install all DLLs
    foreach(dll ${dlls})
        # Separate filename from path
        get_filename_component(path ${dll} PATH)
        get_filename_component(filename ${dll} NAME)

        # Add current DLL's path to search paths
        foreach(_path ${paths})
            set(${dll}_paths ${${dll}_paths} ${_path}/${path})
        endforeach()

        find_file(${dll}_found ${filename} ${${dll}_paths})

        if(${dll}_found)
            install(FILES ${${dll}_found} DESTINATION ${library_install_dir}/${path})
        else()
            message(FATAL_ERROR "DLL ${dll} needed for bundle not found!")
        endif()
    endforeach()
endfunction()
