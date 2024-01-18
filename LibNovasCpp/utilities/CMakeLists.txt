# **********************************************************************************************************************
# Updated 19/01/2024
# **********************************************************************************************************************

# **********************************************************************************************************************
# NOVAS UTILITIES CMAKELIST
# **********************************************************************************************************************

# ----------------------------------------------------------------------------------------------------------------------
# UTILITIES CONFIGURATION

# Log.
message(STATUS "Setup library utilities...")

# Set the installation example path.
set(UTILS_INSTALL_PATH ${MODULES_GLOBAL_INSTALL_BIN_PATH}/utilities)

# Configure names and build path.
set(APP_BUILD_FOLDER ${CMAKE_BINARY_DIR}/bin/Utilities/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APP_BUILD_FOLDER})

# ----------------------------------------------------------------------------------------------------------------------
# Util_GenerateStarAzEl

# Set the target.
set(UTIL_NAME "Util_GenerateStarAzEl")

# Set the sources.
set(SOURCES "GenerateStarAzEl/Util_GenerateStarAzEl.cpp")

# Include the external resources.
if(MODULES_GLOBAL_SHOW_EXTERNALS)
    file(GLOB_RECURSE EXTERN ${CMAKE_SOURCE_DIR}/includes/*.h)
endif()

# Setup the launcher.
macro_setup_launcher("${UTIL_NAME}"
                     "${MODULES_GLOBAL_LIBS_OPTIMIZED}"
                     "${MODULES_GLOBAL_LIBS_DEBUG}"
                     "${SOURCES}" "${EXTERN}")

# Include directories for the target.
target_include_directories(${UTIL_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/includes)

# Install the launcher.
macro_install_launcher(${UTIL_NAME} ${UTILS_INSTALL_PATH})

# Install runtime artifacts.
macro_install_runtime_artifacts(${UTIL_NAME}
                                ${MODULES_GLOBAL_MAIN_DEP_SET_NAME}
                                ${UTILS_INSTALL_PATH})

# Install the runtime dependencies.
macro_install_runtime_deps("${UTIL_NAME}"
                           "${MODULES_GLOBAL_MAIN_DEP_SET_NAME}"
                           "${CMAKE_BINARY_DIR}/bin"
                           "${UTILS_INSTALL_PATH}"
                           "" "")

# ----------------------------------------------------------------------------------------------------------------------
