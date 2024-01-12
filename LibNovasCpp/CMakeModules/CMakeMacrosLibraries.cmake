# **********************************************************************************************************************
# Updated 12/01/2024
# **********************************************************************************************************************

# **********************************************************************************************************************

# NOTE:Add all resources as last arguments.
MACRO(macro_setup_shared_lib lib_name lib_includes_dir)

    # Combine all additional arguments into a single list of resources.
    set(resources ${ARGN})

    # Log.
    message(STATUS "Setup shared library: ${lib_name}")

    # Add definitions and the library.
    string(TOUPPER ${lib_name} LIB_NAME_UPPER)
    add_definitions(-D${LIB_NAME_UPPER}_LIBRARY)
    add_library(${lib_name} SHARED ${resources})
    target_compile_definitions(${lib_name} PRIVATE -D${LIB_NAME_UPPER}_LIBRARY)

    # Add the includes.
    target_include_directories(${lib_name} PUBLIC
                               $<BUILD_INTERFACE:${lib_includes_dir}>
                               $<BUILD_INTERFACE:${lib_includes_dir}/..>
                               $<INSTALL_INTERFACE:include>)

ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_setup_lib_basic_examples lib_dependencies)

    # Log.
    message(STATUS "Setup library examples...")

    # Configure build path.
    set(APP_BUILD_FOLDER ${CMAKE_BINARY_DIR}/bin/Examples/)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APP_BUILD_FOLDER})

    # Empty lib lists.
    set(LIB_DEB)
    set(LIB_OPT)

    # Setup the library.
    # Check if debug or release.
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(LIB_DEB ${LIB_NAME})
    else()
        set(LIB_OPT ${LIB_NAME})
    endif()

    # List of basic tests.
    file(GLOB EXAMPLE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

    # Loop through the test names and configure each basic test.
    foreach(EXAMPLE_SOURCE_FILE ${EXAMPLE_SOURCES})
        get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE_FILE} NAME_WE)
        set(SOURCES ${EXAMPLE_NAME}.cpp)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APP_BUILD_FOLDER})
        # Call the macro to set up the test
        #macro_setup_deploy_launcher("${EXAMPLE_NAME}" "${INSTALL_BIN}" "${LIB_DEPS_SET}")

        if(MODULES_GLOBAL_SHOW_EXTERNALS)
            file(GLOB_RECURSE EXTERN ${CMAKE_SOURCE_DIR}/includes/*.h)
        endif()

        macro_setup_launcher_deploy("${EXAMPLE_NAME}" "${LIB_OPT}" "${LIB_DEB}" "${INSTALL_BIN}" "${LIB_DEPS_SET}"
                                    "${SOURCES}" "${EXTERN}")

        target_include_directories(${EXAMPLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/includes)

    endforeach()

ENDMACRO()

# **********************************************************************************************************************
