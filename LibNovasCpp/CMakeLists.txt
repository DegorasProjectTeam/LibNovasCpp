# **********************************************************************************************************************
# Updated 14/12/2023
# **********************************************************************************************************************

# Cmake minimum.
cmake_minimum_required(VERSION 3.21)

# Check version.
if (NOT ${CMAKE_VERSION} VERSION_LESS "3.1.0")
    cmake_policy(SET CMP0054 NEW)
endif()

# Library names.
set(LIB_FULL_NAME LibNovasCpp)
set(LIB_SHRT_NAME LibNovasCpp)
set(LIB_NOVASCPP ${LIB_FULL_NAME})

# Set the version.
# NOTE: 3.1 is NOVAS C3.1
set(MAJOR_VERSION 3)
set(MINOR_VERSION 1)
set(VERSION ${MAJOR_VERSION}.${MINOR_VERSION})

# Project name.
project(${LIB_FULL_NAME} VERSION ${MAJOR_VERSION}.${MINOR_VERSION})

# ----------------------------------------------------------------------------------------------------------------------
# Log the compiler, SO and architecture.
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
if(WIN32)
    set(OS_NAME "Windows")
elseif(APPLE)
    set(OS_NAME "Apple")
elseif(UNIX AND NOT APPLE)
    set(OS_NAME "Linux/Unix")
else()
    set(OS_NAME "Unknown")
endif()
message(STATUS "Operating System: ${OS_NAME}")
message(STATUS "System Architecture: ${CMAKE_SYSTEM_PROCESSOR}")
# ----------------------------------------------------------------------------------------------------------------------

#Configure the build type.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Check the architecture.
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Architecture not supported by default.")
endif()

# Set path to additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH})
include(CMakeMacrosInstall)
include(CMakeMacrosLauncher)
include(CMakeMacrosUtils)

# Output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Default installation dir.
set(DEF_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../${LIB_FULL_NAME}_product_v${VERSION})

# Set the cmake config file name.
set(LIB_CMAKE_CONFIG ${LIB_FULL_NAME}_config)

# Set the library includes folders.
set(LIB_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/includes/)
set(LIB_INCLUDES_FULL ${CMAKE_CURRENT_SOURCE_DIR}/includes/${LIB_FULL_NAME})

# Other variables setting.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(SHOW_EXTERNALS TRUE)
set(FORCE_INSTALL_DIR TRUE)
set(INSTALL_EXT_DEPS TRUE)
set(LIB_DEPS_SET ${LIB_FULL_NAME}_deps)
set(DEP_DIRS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Define the install directory for each compiler and operating system.
macro_prepare_install_dir(${DEF_INSTALL_DIR} INSTALL_BIN INSTALL_LIB INSTALL_SHARE INSTALL_INCLUDE)

# SOLAR SYSTEM versions.
set(SOLSYS1 ${CMAKE_SOURCE_DIR}/sources/solsys/solsys1.c)
set(SOLSYS2 ${CMAKE_SOURCE_DIR}/sources/solsys/solsys2.c)
set(SOLSYS3 ${CMAKE_SOURCE_DIR}/sources/solsys/solsys3.c)

# Configure the SOLAR SYSTEM version to be used.
# NOTE: Remember compile Fortran code if you want to use full accuracy mode.
set(SOLSYS_SOURCE ${SOLSYS3})

# Get the headers, sources and template files.
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/includes/*.h)
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/sources/*.c)
set(SOURCES ${SOURCES} ${SOLSYS_SOURCE})

# Add definitions and the library.
string(TOUPPER ${LIB_FULL_NAME} LIB_FULL_NAME_UPPER)
add_definitions(-D${LIB_FULL_NAME_UPPER}_LIBRARY)
add_library(${LIB_FULL_NAME} SHARED ${SOURCES} ${HEADERS}
    includes/LibNovasCpp/novascpp.h)
target_compile_definitions(${LIB_FULL_NAME} PRIVATE -D${LIB_FULL_NAME_UPPER}_LIBRARY)

# Configure the compilers.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -O3")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${LIB_FULL_NAME} PRIVATE /std:c++17)
else()
    message(FATAL_ERROR "Compiler not supported by default.")
endif()

# Add the includes.
target_include_directories(${LIB_FULL_NAME} PUBLIC
                           $<BUILD_INTERFACE:${LIB_INCLUDES_FULL}>
                           $<BUILD_INTERFACE:${LIB_INCLUDES}>
                           $<INSTALL_INTERFACE:include>)

# In mingw better do static linking of the libgcc, libwinpthread and libstd.
if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static -lpthread")
    target_link_libraries(${LIB_FULL_NAME} PRIVATE -static-libgcc -static-libstdc++ -static -lpthread)
endif()

# Add the diferents subprojects directories.
add_subdirectory(examples)
add_subdirectory(utilities)

# Installation process.
if(WIN32)

    macro_install_lib(${LIB_FULL_NAME} ${LIB_CMAKE_CONFIG} ${LIB_INCLUDES_FULL} ${INSTALL_INCLUDE}
                      ${INSTALL_LIB} ${INSTALL_LIB} ${INSTALL_LIB} ${INSTALL_SHARE})

    macro_install_artifacts(${LIB_FULL_NAME} ${LIB_DEPS_SET} ${INSTALL_BIN})

    if(${INSTALL_EXT_DEPS})
        set(EXC_REGEXES "api-ms-" "ext-ms-")
        macro_install_ext_deps(${LIB_DEPS_SET} ${DEP_DIRS} ${INSTALL_BIN} "${EXC_REGEXES}")
    endif()
    
elseif(OS_NAME STREQUAL "Linux/Unix")

    macro_install_lib(${LIB_FULL_NAME} ${LIB_CMAKE_CONFIG} ${LIB_INCLUDES_FULL} ${INSTALL_INCLUDE} 
                      ${INSTALL_LIB} ${INSTALL_LIB} ${INSTALL_LIB} ${INSTALL_SHARE})
                      
    macro_install_artifacts(${LIB_FULL_NAME} ${LIB_DEPS_SET} ${INSTALL_BIN})
    
    if(${INSTALL_EXT_DEPS})
        macro_install_ext_deps(${LIB_DEPS_SET} ${DEP_DIRS} ${INSTALL_BIN} "${EXC_REGEXES}")
    endif()
                  
else()
    message(FATAL_ERROR "Operating system not supported by default.")
endif()
