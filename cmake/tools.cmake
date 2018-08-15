if(COMMAND configure_and_install)
    # header included already
    return()
endif()

function(configure_and_install _project_name _version_compare_rules)
    # set variables
    include(CMakePackageConfigHelpers)
    set(ConfigPackageSource ${CMAKE_CURRENT_BINARY_DIR})
    set(ConfigPackageDestination lib/cmake/${_project_name})
    write_basic_package_version_file(
            ${ConfigPackageSource}/${_project_name}-config-version.cmake
            COMPATIBILITY ${_version_compare_rules})

    # install
    install(TARGETS ${_project_name} EXPORT ${_project_name}Targets
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
            INCLUDES DESTINATION include)
    install(EXPORT ${_project_name}Targets
            DESTINATION ${ConfigPackageDestination}
            FILE ${_project_name}-targets.cmake
            NAMESPACE Linux13524::)
    install(FILES
            "cmake/${_project_name}-config.cmake"
            "${ConfigPackageSource}/${_project_name}-config-version.cmake"
            DESTINATION ${ConfigPackageDestination}
            COMPONENT Devel)
    install(DIRECTORY include/${_project_name} DESTINATION include)
endfunction()