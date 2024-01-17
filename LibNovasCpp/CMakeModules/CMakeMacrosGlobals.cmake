# **********************************************************************************************************************
# Updated 12/01/2024
# **********************************************************************************************************************

# **********************************************************************************************************************

MACRO(macro_cmakemodules_init)

    # Includes.
    include(CMakeMacrosInstall)
    include(CMakeMacrosLauncher)
    include(CMakeMacrosUtils)
    include(CMakeMacrosProjects)
    include(CMakeMacrosLibraries)

    # Default global variables values.
    macro_global_set_main_dep_set_name("")
    macro_global_set_libs_debug("")
    macro_global_set_libs_optimized("")
    macro_global_set_install_bin_path("")
    macro_global_set_install_lib_path("")
    macro_global_set_install_share_path("")
    macro_global_set_install_include_path("")
    macro_global_set_install_include_path("")
    macro_global_set_show_externals(FALSE)
    macro_global_set_force_install_dir(TRUE)
    macro_global_set_install_ext_deps(FALSE)
    macro_global_set_install_runtime_artifacts(TRUE)

ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_global_set_main_dep_set_name dep_set_name)

    set(MODULES_GLOBAL_MAIN_DEP_SET_NAME ${dep_set_name})

ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_global_set_libs_debug lib_names)

    set(MODULES_GLOBAL_LIBS_DEBUG ${lib_names})

ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_global_set_libs_optimized lib_names)

    set(MODULES_GLOBAL_LIBS_OPTIMIZED ${lib_names})

ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_global_add_libs_debug lib_names)

set(MODULES_GLOBAL_LIBS_DEBUG ${MODULES_GLOBAL_LIBS_DEBUG} ${lib_names})

ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_global_add_libs_optimized lib_names)

set(MODULES_GLOBAL_LIBS_OPTIMIZED ${MODULES_GLOBAL_LIBS_OPTIMIZED} ${lib_names})

ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_global_set_install_bin_path path)
    set(MODULES_GLOBAL_INSTALL_BIN_PATH ${path})
ENDMACRO()

MACRO(macro_global_set_install_lib_path path)
    set(MODULES_GLOBAL_INSTALL_LIB_PATH ${path})
ENDMACRO()

MACRO(macro_global_set_install_share_path path)
    set(MODULES_GLOBAL_INSTALL_SHARE_PATH ${path})
ENDMACRO()

MACRO(macro_global_set_install_include_path path)
    set(MODULES_GLOBAL_INSTALL_INCLUDE_PATH ${path})
ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_global_set_show_externals enabled)
    set(MODULES_GLOBAL_SHOW_EXTERNALS ${enabled})
ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_global_set_force_install_dir enabled)
    set(MODULES_GLOBAL_FORCE_INSTALL_DIR ${enabled})
ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_global_set_install_ext_deps enabled)
    set(MODULES_GLOBAL_INSTALL_EXT_DEPS ${enabled} )
ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_global_set_install_runtime_artifacts enabled)
    set(MODULES_GLOBAL_INSTALL_RUNTIME_ARTIFACTS ${enabled})
ENDMACRO()

# **********************************************************************************************************************
