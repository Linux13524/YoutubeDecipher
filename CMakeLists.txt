cmake_minimum_required(VERSION 3.11)
project(Youtube_Decipher)

set(CMAKE_CXX_STANDARD 11)

if (MINGW)
    set(Platform_LINKAGE ws2_32)
elseif (ANDROID)
    set(BOOST_ROOT ${ANDROID_NDK}/libs/Boost/1.66.0/${ANDROID_ABI})
    set(OPENSSL_ROOT_DIR ${ANDROID_NDK}/libs/OpenSSL/1.1.0h/${ANDROID_ABI})
    set(Platform_LINKAGE log)
endif ()

### OPENSSL - static
set(OPENSSL_USE_STATIC_LIBS ON)
find_package(OpenSSL REQUIRED)

set(OpenSSL_LINKAGE OpenSSL::SSL OpenSSL::Crypto)


### BOOST - static
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system regex REQUIRED)

set(CMAKE_USE_LIBSSH2 ON)
set(CMAKE_USE_OPENSSL ON)

set(Boost_LINKAGE Boost::system Boost::regex )

### Youtube-Decipher build
file(GLOB_RECURSE Headers
        include/*.h
        lib/HttpsGetProvider/*.hpp
        lib/json/*.hpp)
file(GLOB_RECURSE Sources src/*.cpp)

include_directories(
        include
        include/utils/json
        ${Boost_INCLUDE_DIRS}
        lib)

add_library(Youtube_Decipher STATIC ${Headers} ${Sources})

target_link_libraries(Youtube_Decipher PRIVATE  ${Boost_LINKAGE} ${OpenSSL_LINKAGE} ${Platform_LINKAGE})
