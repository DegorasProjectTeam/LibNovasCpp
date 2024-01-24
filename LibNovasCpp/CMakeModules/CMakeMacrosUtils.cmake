# **********************************************************************************************************************
# Updated 24/01/2024
# **********************************************************************************************************************

# **********************************************************************************************************************

MACRO(macro_get_files_without_extension result curdir)
    file(GLOB_RECURSE FILES ${curdir})
    foreach(FILE ${FILES})
        get_filename_component(FILENAME ${FILE} NAME)
        if(${FILENAME} MATCHES "^[^.]+$")
            list(APPEND FILELIST ${FILE})
        endif()
    endforeach()
    set(${result} ${FILELIST})
ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_subdir_list result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
        list(APPEND dirlist ${child})
    endif()
    endforeach()
    set(${result} ${dirlist})
ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_add_subdirs curdir)
    macro_subdir_list(subdirs ${curdir})
    foreach(subdir ${subdirs})
      add_subdirectory(${subdir})
    endforeach()
ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_compose_current_architecture_folder_name result_var)

    # Check if 64-bit or 32-bit
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
        message(FATAL_ERROR "Compiler not supported by default.")
    endif()

    # Construct the configuration folder name
    set(${result_var} "${COMP_N}-${ARCH}-${COMP_V}-${BUILD_TYPE}")

ENDMACRO()

# **********************************************************************************************************************
