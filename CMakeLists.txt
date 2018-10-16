cmake_minimum_required(VERSION 3.6)
project(YoutubeDecipher)

include(cmake/tools.cmake)
include(cmake/conan.cmake)

set(PROJECT_VERSION 1.0.0)
set(CMAKE_DEBUG_POSTFIX "-d")

if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

if (MINGW)
    set(Platform_LINKAGE ws2_32)
    set(CMAKE_EXE_LINKER_FLAGS "-static") # link everything statically so the user does not need to install MinGW
    set(CONAN_SETTINGS compiler.exception=seh compiler.threads=posix)
elseif(ANDROID)
    set(Platform_LINKAGE log)
    set(CMAKE_CXX_COMPILER_VERSION "5.0")
endif()

### Library settings
set(CMAKE_USE_LIBSSH2 ON)
set(CMAKE_USE_OPENSSL ON)
set(OPENSSL_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_LIBS ON)

### Run conan install to get dependencies
conan_cmake_run(CONANFILE conanfile.txt
        BASIC_SETUP
        SETTINGS ${CONAN_SETTINGS})


### Youtube-Decipher build
set(Headers
        include/YoutubeDecipher/decipher.h
        lib/HttpsGetProvider/json.hpp
        lib/HttpsGetProvider/networking.hpp
        lib/HttpsGetProvider/root_certificates.hpp
        lib/json/json.hpp)

set(Sources
        src/youtube/decipher.cpp)

add_library(YoutubeDecipher STATIC
        ${Headers}
        ${Sources})

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
