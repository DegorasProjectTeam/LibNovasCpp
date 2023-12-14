# **********************************************************************************************************************
# Updated 14/12/2023
# **********************************************************************************************************************

# **********************************************************************************************************************

MACRO(macro_setup_launcher LAUNCHER_NAME)

    message(STATUS "${Green}Preparing executable: ${LAUNCHER_NAME} ${ColourReset}")

    # Link the current dirs.
    if (WIN32)
        link_directories(${CMAKE_CURRENT_BINARY_DIR})
    elseif(UNIX)
        link_directories(${CMAKE_CURRENT_BINARY_DIR}/../lib)
    endif()

    add_executable(${LAUNCHER_NAME} ${SOURCES} ${HEADERS} ${TEMPLTS} ${ALIAS} ${EXTERN} ${RESOURCES})

    if(LIBRARIES)
        message(STATUS "${Green}  Linking general libraries for the executable. ${ColourReset}")
        target_link_libraries(${LAUNCHER_NAME} PRIVATE ${LIBRARIES})
    endif()

    if(LIBRARIES_OPTIMIZED)
        message(STATUS "${Green}  Linking optimized libraries for the executable. ${ColourReset}")
        foreach(LIBOPT ${LIBRARIES_OPTIMIZED})
            target_link_libraries(${LAUNCHER_NAME} optimized ${LIBOPT})
        endforeach(LIBOPT)
    endif(LIBRARIES_OPTIMIZED)

    if(LIBRARIES_DEBUG)
        message(STATUS "${Green}  Linking debug libraries for the executable. ${ColourReset}")
        foreach(LIBDEBUG ${LIBRARIES_DEBUG})
            target_link_libraries(${LAUNCHER_NAME} debug ${LIBDEBUG})
        endforeach(LIBDEBUG)
    endif(LIBRARIES_DEBUG)

    # Set target properties.
    message(STATUS "${Green}  Setting target properties. ${ColourReset}")
    set_target_properties(${LAUNCHER_NAME} PROPERTIES PROJECT_LABEL "Launcher ${LAUNCHER_NAME}")
    set_target_properties(${LAUNCHER_NAME} PROPERTIES DEBUG_OUTPUT_NAME "${LAUNCHER_NAME}${CMAKE_DEBUG_POSTFIX}")
    set_target_properties(${LAUNCHER_NAME} PROPERTIES RELEASE_OUTPUT_NAME "${LAUNCHER_NAME}${CMAKE_RELEASE_POSTFIX}")
    set_target_properties(${LAUNCHER_NAME} PROPERTIES RELWITHDEBINFO_OUTPUT_NAME "${LAUNCHER_NAME}${CMAKE_RELWITHDEBINFO_POSTFIX}")
    set_target_properties(${LAUNCHER_NAME} PROPERTIES MINSIZEREL_OUTPUT_NAME "${LAUNCHER_NAME}${CMAKE_MINSIZEREL_POSTFIX}")
    set_target_properties(${LAUNCHER_NAME} PROPERTIES FOLDER CMAKE_RUNTIME_OUTPUT_DIRECTORY)

ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_deploy_launcher LAUNCHER_NAME INSTALL_PATH)

    message(STATUS "${Green}Preparing the deployment of the executable: ${LAUNCHER_NAME} ${ColourReset}")

    #Deploy binary files
    install(TARGETS ${LAUNCHER_NAME}
            RUNTIME DESTINATION ${INSTALL_PATH})

ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_deploy_runtime_artifacts LAUNCHER_NAME INSTALL_PATH DEPENDENCY_SET)

    message(STATUS "${Green}Preparing the runtime artifacts of the executable: ${LAUNCHER_NAME} ${ColourReset}")

    # Installation process into installation dir.
    install(IMPORTED_RUNTIME_ARTIFACTS ${LAUNCHER_NAME}
            RUNTIME_DEPENDENCY_SET ${DEPENDENCY_SET}
            DESTINATION ${INSTALL_PATH})

ENDMACRO()

# **********************************************************************************************************************

MACRO(macro_setup_deploy_launcher LAUNCHER_NAME INSTALL_PATH DEPENDENCY_SET)

    macro_setup_launcher("${LAUNCHER_NAME}")
    macro_deploy_launcher("${LAUNCHER_NAME}" "${INSTALL_PATH}")
    macro_deploy_runtime_artifacts("${LAUNCHER_NAME}" "${INSTALL_PATH}" "${DEPENDENCY_SET}")

ENDMACRO()

# **********************************************************************************************************************
