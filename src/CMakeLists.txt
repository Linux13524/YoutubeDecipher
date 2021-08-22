cmake_minimum_required(VERSION 3.6)
project(YoutubeDecipher)

include(cmake/tools.cmake)
include(cmake/conan.cmake)

set(PROJECT_VERSION 1.0.1)

if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-static") # link everything statically so the user does not need to install MinGW
    set(CONAN_SETTINGS compiler.exception=seh compiler.threads=posix)
elseif(ANDROID)
    set(Platform_LINKAGE log)
    # May be removed?
    # set(CMAKE_CXX_COMPILER_VERSION "7.0")
endif()

### CONAN
# Runs conan install to get dependencies
conan_cmake_run(CONANFILE ../conanfile.py
        BASIC_SETUP
        SETTINGS ${CONAN_SETTINGS})

### Youtube-Decipher build
set(Headers
        include/YoutubeDecipher/decipher.h
        ${CONAN_INCLUDE_DIRS})

set(Sources
        youtube/decipher.cpp)

add_library(YoutubeDecipher STATIC
        ${Headers}
        ${Sources})

set_target_properties(YoutubeDecipher PROPERTIES DEBUG_POSTFIX "-d")

if (CMAKE_VERSION VERSION_LESS "3.8")
    set(CMAKE_CXX_STANDARD 11)
else()
    target_compile_features(YoutubeDecipher PUBLIC cxx_std_11)
endif()

target_include_directories(YoutubeDecipher
        INTERFACE $<INSTALL_INTERFACE:include>
        INTERFACE $<INSTALL_INTERFACE:lib>
        PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
        PRIVATE $<BUILD_INTERFACE:${CONAN_INCLUDE_DIRS}>)

target_link_libraries(YoutubeDecipher
        PRIVATE ${CONAN_LIBS}
        PRIVATE ${Platform_LINKAGE})

add_library(Linux13524::YoutubeDecipher ALIAS YoutubeDecipher)

### For installation of the library
install(TARGETS YoutubeDecipher EXPORT YoutubeDecipherTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include)
install(DIRECTORY include/YoutubeDecipher DESTINATION include)

configure_and_install(YoutubeDecipher SameMajorVersion)
