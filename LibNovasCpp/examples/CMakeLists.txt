# **********************************************************************************************************************
# Updated 27/11/2023
# **********************************************************************************************************************

# **********************************************************************************************************************

# Configure names and build path.
set(APP_BUILD_FOLDER ${CMAKE_BINARY_DIR}/bin/Examples/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APP_BUILD_FOLDER})

# Include directories.
include_directories(${CMAKE_SOURCE_DIR}/examples)
include_directories(${CMAKE_SOURCE_DIR}/includes)

# Setup the libraries.
set(LIBRARIES ${LIB_NOVASCPP})

# List of tests.
set(EXAMPLE_NAMES
    "Example_GenericCalculations")

# Loop through the test names and configure each test
foreach(EXAMPLE_NAME ${EXAMPLE_NAMES})
    set(SOURCES "${EXAMPLE_NAME}.cpp")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APP_BUILD_FOLDER})
    # Call the macro to set up the test
    macro_setup_deploy_launcher("${EXAMPLE_NAME}" "${INSTALL_BIN}" "${LIB_DEPS_SET}")
    target_include_directories(${EXAMPLE_NAME} PRIVATE
                               ${CMAKE_SOURCE_DIR}/includes)
endforeach()
