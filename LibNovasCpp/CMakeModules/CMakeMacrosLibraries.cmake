# **********************************************************************************************************************
# Updated 19/01/2024
# **********************************************************************************************************************

# **********************************************************************************************************************

# Note: Add all resources as last arguments.
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

   # Append the new library to global.
   if(CMAKE_BUILD_TYPE STREQUAL "Debug")
       macro_global_add_libs_debug("${lib_name}")
   else()
       macro_global_add_libs_optimized("${lib_name}")
   endif()

   # Define the global dependency set name.
   macro_global_set_main_dep_set_name("${lib_name}_deps")

ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_setup_lib_basic_examples examples_sources_path examples_install_path)

    # Log.
    message(STATUS "Setup library examples...")

    # Configure build path.
    set(APP_BUILD_FOLDER ${CMAKE_BINARY_DIR}/bin/Examples/)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APP_BUILD_FOLDER})

    # List of basic tests.
    file(GLOB EXAMPLE_SOURCES "${examples_sources_path}/*.cpp")

    # Loop through the test names and configure each basic test.
    foreach(EXAMPLE_SOURCE_FILE ${EXAMPLE_SOURCES})

        # Get the test name and source.
        get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE_FILE} NAME_WE)
        set(SOURCES ${EXAMPLE_NAME}.cpp)

        # Include the external resources.
        if(MODULES_GLOBAL_SHOW_EXTERNALS)
            file(GLOB_RECURSE EXTERN ${CMAKE_SOURCE_DIR}/includes/*.h)
        endif()

        # Setup the launcher.
        macro_setup_launcher("${EXAMPLE_NAME}"
                             "${MODULES_GLOBAL_LIBS_OPTIMIZED}"
                             "${MODULES_GLOBAL_LIBS_DEBUG}"
                             "${SOURCES}" "${EXTERN}")

        # Include directories for the target.
        target_include_directories(${EXAMPLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/includes)

        # Install the launcher.
        macro_install_launcher(${EXAMPLE_NAME} ${examples_install_path})

        # Install runtime artifacts.
        macro_install_runtime_artifacts(${EXAMPLE_NAME}
                                        ${MODULES_GLOBAL_MAIN_DEP_SET_NAME}
                                        ${examples_install_path})

        # Install the runtime dependencies.
        macro_install_runtime_deps("${EXAMPLE_NAME}"
                                   "${MODULES_GLOBAL_MAIN_DEP_SET_NAME}"
                                   "${CMAKE_BINARY_DIR}/bin"
                                   "${examples_install_path}"
                                   "" "")

    endforeach()

ENDMACRO()

# **********************************************************************************************************************
