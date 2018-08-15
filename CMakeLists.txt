cmake_minimum_required(VERSION 3.6)
project(YoutubeDecipher)

include(cmake/tools.cmake)

set(PROJECT_VERSION 1.0.0)
set(CMAKE_DEBUG_POSTFIX "-d")

if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

if (ANDROID)
    set(BOOST_ROOT ${ANDROID_NDK}/libs/Boost/${ANDROID_ABI})
    set(OPENSSL_ROOT_DIR ${ANDROID_NDK}/libs/OpenSSL/${ANDROID_ABI})
endif ()

### OPENSSL - static
set(OPENSSL_USE_STATIC_LIBS ON)
find_package(OpenSSL REQUIRED)


### BOOST - static
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.66 COMPONENTS system regex REQUIRED)

set(CMAKE_USE_LIBSSH2 ON)
set(CMAKE_USE_OPENSSL ON)


### Youtube-Decipher build
add_library(YoutubeDecipher STATIC
        include/YoutubeDecipher/decipher.h
        lib/HttpsGetProvider/json.hpp
        lib/HttpsGetProvider/networking.hpp
        lib/HttpsGetProvider/root_certificates.hpp
        lib/json/json.hpp
        src/youtube/decipher.cpp)

target_compile_features(YoutubeDecipher PUBLIC cxx_std_11)

target_include_directories(YoutubeDecipher PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
            $<INSTALL_INTERFACE:include>
            $<INSTALL_INTERFACE:lib>
            $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
            $<BUILD_INTERFACE:${OPENSSL_INCLUDE_DIR}>)

target_link_libraries(YoutubeDecipher
        PRIVATE ${Boost_LIBRARIES}
        PRIVATE ${OPENSSL_LIBRARIES})

if (MINGW)
    target_link_libraries(YoutubeDecipher PRIVATE ws2_32)
elseif (ANDROID)
    target_link_libraries(YoutubeDecipher PRIVATE log)
endif()

add_library(Linux13524::YoutubeDecipher ALIAS YoutubeDecipher)

set_target_properties(YoutubeDecipher PROPERTIES PUBLIC_HEADER "include/YoutubeDecipher/decipher.h")

### For installation of the library
configure_and_install(SameMajorVersion)
