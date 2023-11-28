# **********************************************************************************************************************
# Updated 27/11/2023
# **********************************************************************************************************************

# Cmake minimum.
cmake_minimum_required(VERSION 3.21)

# Check version.
if (NOT ${CMAKE_VERSION} VERSION_LESS "3.1.0")
    cmake_policy(SET CMP0054 NEW)
endif()

#Configure the build type.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Library names.
set(LIB_FULL_NAME LibNovasCpp)
set(LIB_SHRT_NAME LibNovasCpp)
set(LIB_NOVASCPP ${LIB_FULL_NAME})

# Project name.
project(${LIB_FULL_NAME})

# Check the architecture.
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Architecture not supported by default.")
endif()

# Set path to additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH})
include(CMakeMacros)

# Set the version.
set(MAJOR_VERSION 3)
set(MINOR_VERSION 1)
set(PATCH_VERSION C)
set(VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")

# Output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Other variables setting.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(SHOW_EXTERNALS TRUE)
set(FORCE_INSTALL_DIR TRUE)
set(LIB_DEPS_SET ${LIB_FULL_NAME}_deps)

# Define the install directory for each compiler and operating system.
macro_prepare_install_dir(INSTALL_BIN INSTALL_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../product_v${VERSION})

# SOLAR SYSTEM versions.
set(SOLSYS1 ${CMAKE_SOURCE_DIR}/sources/solsys/solsys1.c)
set(SOLSYS2 ${CMAKE_SOURCE_DIR}/sources/solsys/solsys2.c)
set(SOLSYS3 ${CMAKE_SOURCE_DIR}/sources/solsys/solsys3.c)

# Configure the SOLAR SYSTEM version to be used.
set(SOLSYS_SOURCE ${SOLSYS3})

# Get the headers, sources and template files.
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/includes/*.h)
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/sources/*.c)
set(SOURCES
    ${SOURCES}
    ${SOLSYS_SOURCE})

# Add definitions and the library.
string(TOUPPER ${LIB_FULL_NAME} LIB_FULL_NAME_UPPER)
add_definitions(-D${LIB_FULL_NAME_UPPER}_LIBRARY)
add_library(${LIB_FULL_NAME} SHARED ${SOURCES} ${HEADERS})
target_compile_definitions(${LIB_FULL_NAME} PRIVATE -D${LIB_FULL_NAME_UPPER}_LIBRARY)

# Add the includes.
target_include_directories(${LIB_FULL_NAME} PRIVATE
                           ${CMAKE_SOURCE_DIR}/includes
                           ${CMAKE_SOURCE_DIR}/includes/${LIB_FULL_NAME})

# Configure the compilers.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -O3")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${LIB_FULL_NAME} PRIVATE /std:c++17)
else()
    message(FATAL_ERROR "Compiler not supported by default.")
endif()

# In mingw better do static linking of the libgcc, libwinpthread and libstd.
if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static -lpthread")
    target_link_libraries(${LIB_FULL_NAME} PRIVATE -static-libgcc -static-libstdc++ -static -lpthread)
endif()

# Add the diferents subprojects directories.
add_subdirectory(examples)
add_subdirectory(utilities)

# Installation process for windows.
if(WIN32)

    # Install the include files.
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/includes/${LIB_FULL_NAME}
            DESTINATION ${CMAKE_INSTALL_PREFIX}/includes
            PATTERN "*.txt" EXCLUDE)

    # Install the binaries to the specified install directory.
    install(TARGETS ${LIB_FULL_NAME}
            LIBRARY DESTINATION ${INSTALL_LIB}
            RUNTIME DESTINATION ${INSTALL_LIB})

    # Install the lib files for msvc.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

        # Install the LibZMQUtils.lib file to the specified library directory.
        install(FILES ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${LIB_FULL_NAME}.lib
                DESTINATION ${INSTALL_LIB})

    endif()

    # Runtime artifacts.
    install(IMPORTED_RUNTIME_ARTIFACTS ${LIB_FULL_NAME}
            RUNTIME_DEPENDENCY_SET ${LIB_DEPS_SET}
            DESTINATION ${INSTALL_BIN})

endif()
