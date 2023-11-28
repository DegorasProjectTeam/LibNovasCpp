# **********************************************************************************************************************
# Updated 27/11/2023
# **********************************************************************************************************************

# **********************************************************************************************************************

# Configure names and build path.
set(APP_BUILD_FOLDER ${CMAKE_BINARY_DIR}/bin/Utilities/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APP_BUILD_FOLDER})

# Include directories.
include_directories(${CMAKE_SOURCE_DIR}/utilities)
include_directories(${CMAKE_SOURCE_DIR}/includes)

# Setup the libraries.
set(LIBRARIES ${LIB_NOVASCPP})

set(SOURCES "GenerateStarAzEl/Util_GenerateStarAzEl.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APP_BUILD_FOLDER})

# Call the macro to set up the test
macro_setup_deploy_launcher("Util_GenerateStarAzEl" "${INSTALL_BIN}" "${LIB_DEPS_SET}")

target_include_directories(Util_GenerateStarAzEl PRIVATE
                           ${CMAKE_SOURCE_DIR}/includes)
