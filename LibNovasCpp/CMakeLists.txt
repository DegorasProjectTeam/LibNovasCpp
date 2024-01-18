# **********************************************************************************************************************
# Updated 12/01/2024
# **********************************************************************************************************************

# **********************************************************************************************************************
# NOVAS MAIN SHARED LIBRARY PROJECT CMAKELIST
# **********************************************************************************************************************

# ----------------------------------------------------------------------------------------------------------------------
#
# INITIALIZATION

# Cmake minimum and empty project for avoid warnings.
cmake_minimum_required(VERSION 3.21)
project(_)

# Set path to additional CMake modules.
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH})
include(CMakeMacrosGlobals)

# Init CMake modules.
macro_cmakemodules_init()

# ----------------------------------------------------------------------------------------------------------------------
# BASIC PROJECT CONFIGURATION

# Base project configuration.
set(LIB_NAME LibNovasCpp)
set(LIB_VER 3.1) # 3.1 is NOVAS C3.1
set(LIB_BUILD_MODE Debug)
set(LIB_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../${LIB_NAME}_product_v${LIB_VER})
set(LIB_CMAKE_CONFIG_NAME ${LIB_NAME}_config)

# Macros global configuration.
macro_global_set_show_externals(FALSE)
macro_global_set_force_install_dir(TRUE)
macro_global_set_install_runtime_deps(TRUE)
macro_global_set_install_runtime_artifacts(TRUE)
macro_global_set_exclude_so_libraries(TRUE)

# Setup the base project.
macro_setup_base_project(${LIB_NAME} ${LIB_VER} ${LIB_BUILD_MODE})

# Other configurations.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_SHARED_LIBRARY_PREFIX "")

# Setup the install directory for each compiler and operating system.
macro_setup_install_dir(${LIB_INSTALL_DIR})

# ----------------------------------------------------------------------------------------------------------------------
# SPECIFIC PROJECT CONFIGURATION

# Specific configuration.
set(LIB_INCLUDES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/includes/${LIB_NAME})
set(LIB_SOURCES_DIR ${CMAKE_SOURCE_DIR}/sources)

# Configure the SOLAR SYSTEM version to be used.
# NOTE: Remember compile Fortran code if you want to use full accuracy mode.
set(SOLSYS1 ${LIB_SOURCES_DIR}/solsys/solsys1.c)
set(SOLSYS2 ${LIB_SOURCES_DIR}/solsys/solsys2.c)
set(SOLSYS3 ${LIB_SOURCES_DIR}/solsys/solsys3.c)
set(SOLSYS_SOURCE ${SOLSYS3})

# ----------------------------------------------------------------------------------------------------------------------
# ADD THE SHARED LIBRARY

# Get the headers, sources and template files.
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/includes/*.h)
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/sources/*.c)
set(SOURCES ${SOURCES} ${SOLSYS_SOURCE})

# Setup the shared library.
macro_setup_shared_lib(${LIB_NAME} ${LIB_INCLUDES_DIR} ${SOURCES} ${HEADERS})

# ----------------------------------------------------------------------------------------------------------------------
# CONFIGURE THE COMPILERS

# Configure the compilers.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -m64 -O3")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${LIB_NAME} PUBLIC)
else()
    message(FATAL_ERROR "Compiler not supported by default.")
endif()

# In mingw better do static linking of the libgcc, libwinpthread and libstd.
if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static -lpthread")
    target_link_libraries(${LIB_NAME} PRIVATE -static-libgcc -static-libstdc++ -static -lpthread)
endif()

# ----------------------------------------------------------------------------------------------------------------------
# INSTALLATION PROCESS

# Installation process.
macro_default_library_installation(${LIB_NAME} ${LIB_CMAKE_CONFIG_NAME} ${LIB_INCLUDES_DIR})

# ----------------------------------------------------------------------------------------------------------------------
# CONFIGURE THE SUBPROJECTS

# Example subproject.
add_subdirectory(examples)

# Utilities subproject.
add_subdirectory(utilities)

# ----------------------------------------------------------------------------------------------------------------------
