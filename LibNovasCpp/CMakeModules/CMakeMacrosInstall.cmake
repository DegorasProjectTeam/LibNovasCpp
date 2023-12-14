# **********************************************************************************************************************
# Updated 14/12/2023
# **********************************************************************************************************************

# **********************************************************************************************************************

MACRO(macro_prepare_install_dir base_dir bin_dir lib_dir sha_dir inc_dir)

    # Log
    message(STATUS "${Green}Preparing the installation directory... ${ColourReset}")

    # Resolve the path
    get_filename_component(RESOLVED_PATH "${base_dir}" REALPATH)

    # Set the installation path.
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT OR ${FORCE_INSTALL_DIR})
        message(STATUS "${Green}  Force the base installation directory: ${RESOLVED_PATH} ${ColourReset}")
        set(CMAKE_INSTALL_PREFIX ${RESOLVED_PATH} CACHE PATH "..." FORCE)
    endif()

    message(STATUS "${Green}  Setting base installation directory: ${RESOLVED_PATH} ${ColourReset}")

    # Add properties for clean process.
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_INSTALL_PREFIX})

    # Check if 64-bit
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH "x86_64")
    else()
        set(ARCH "x86")
    endif()

    # Check if debug or release.
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(BUILD_TYPE "debug")
    else()
        set(BUILD_TYPE "release")
    endif()

    # Get the compiler version.
    set(COMP_V "${CMAKE_CXX_COMPILER_VERSION}")

    # Check the compiler name.
    if(MINGW)
        set(COMP_N "mingw")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(COMP_N "msvc")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(COMP_N "gnu")
    else()
        message(FATAL_ERROR "  Compiler not supported by default.")
    endif()

    # Define the install directories.
    set(${bin_dir} ${CMAKE_INSTALL_PREFIX}/bin/${COMP_N}-${ARCH}-${COMP_V}-${BUILD_TYPE})
    set(${lib_dir} ${CMAKE_INSTALL_PREFIX}/lib/${COMP_N}-${ARCH}-${COMP_V}-${BUILD_TYPE})
    set(${sha_dir} ${CMAKE_INSTALL_PREFIX}/share/${COMP_N}-${ARCH}-${COMP_V}-${BUILD_TYPE})
    set(${inc_dir} ${CMAKE_INSTALL_PREFIX}/include)
    
    # Specific SO configurations.        
    if(WIN32)
    #
    elseif(OS_NAME STREQUAL "Linux/Unix")
    
        # Update RPATH.
        message(STATUS "${Green} "  Updating RPATH. ${ColourReset})
        
         # use, i.e. don't skip the full RPATH for the build tree
        set(CMAKE_SKIP_BUILD_RPATH FALSE)
         
        # when building, don't use the install RPATH already (but later on when installing).
        set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
        set(CMAKE_INSTALL_RPATH ${INSTALL_BIN})
         
        # Add the automatically determined parts of the RPATH which point to directories outside the build tree.
        set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
                  
    else()
        message(FATAL_ERROR "  Operating system not supported by default.")
    endif()

ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_install_lib lib_name cfg_name inc_path
      inc_dest lib_dest bin_dest arch_dest sha_dest)

    # Log
    message(STATUS "${Green}Installing library: ${lib_name} ${ColourReset}")

    # Install the include files to the specified install directory.
    install(DIRECTORY ${inc_path}
            DESTINATION ${inc_dest}
            PATTERN "*.txt" EXCLUDE)

    # Install the binaries to the specified install directory.
    # In Windows is ok install the dll in the lib folder.
    install(TARGETS ${lib_name} EXPORT ${cfg_name}
            LIBRARY DESTINATION ${lib_dest}
            ARCHIVE DESTINATION ${arch_dest}
            RUNTIME DESTINATION ${bin_dest})

    # Export the configuration.
    message(STATUS "${Green}  Exporting the configuration. ${ColourReset}")
    install(EXPORT ${cfg_name} DESTINATION ${sha_dest}/cmake)

ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_install_artifacts lib_name dep_set artf_dest)

    # Log
    message(STATUS "${Green}Installing runtime artifacts for: ${lib_name} ${ColourReset}")

    # Runtime artifacts.
    install(IMPORTED_RUNTIME_ARTIFACTS ${lib_name}
            RUNTIME_DEPENDENCY_SET ${dep_set}
            DESTINATION ${artf_dest})

ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_install_ext_deps dep_set ext_deps_dirs bin_dest exc_regexes)

    message(STATUS "${Green}Installing external dependencies... ${ColourReset}")

    install(RUNTIME_DEPENDENCY_SET ${dep_set}
            PRE_EXCLUDE_REGEXES ${exc_regexes}
            DIRECTORIES ${ext_deps_dirs}
            DESTINATION ${bin_dest})

ENDMACRO()

# **********************************************************************************************************************
