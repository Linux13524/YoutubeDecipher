if(COMMAND configure_and_install)
    # header included already
    return()
endif()

function(configure_and_install _version_compare_rules)
    # set variables
    include(CMakePackageConfigHelpers)
    set(ConfigPackageSource ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME})
    set(ConfigPackageDestination lib/cmake/${CMAKE_PROJECT_NAME})
    write_basic_package_version_file(
            ${ConfigPackageSource}/${CMAKE_PROJECT_NAME}-config-version.cmake
            COMPATIBILITY ${_version_compare_rules})

    # install
    install(TARGETS ${CMAKE_PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
            INCLUDES DESTINATION include)
    install(EXPORT ${CMAKE_PROJECT_NAME}Targets
            DESTINATION ${ConfigPackageDestination}
            FILE ${CMAKE_PROJECT_NAME}-targets.cmake
            NAMESPACE Linux13524::)
    install(FILES
            "cmake/${CMAKE_PROJECT_NAME}-config.cmake"
            "${ConfigPackageSource}/${CMAKE_PROJECT_NAME}-config-version.cmake"
            DESTINATION ${ConfigPackageDestination}
            COMPONENT Devel)
    install(DIRECTORY include/${CMAKE_PROJECT_NAME} DESTINATION include)
endfunction()