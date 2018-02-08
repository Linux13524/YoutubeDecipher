cmake_minimum_required(VERSION 3.9)
project(Youtube_Decipher)

set(CMAKE_CXX_STANDARD 11)

### OPENSSL
find_package(OpenSSL REQUIRED)

set(OpenSSL_LINKAGE OpenSSL::SSL OpenSSL::Crypto)


### BOOST
find_package(Boost COMPONENTS system thread regex log REQUIRED)

set(CMAKE_USE_LIBSSH2 ON)
set(CMAKE_USE_OPENSSL ON)

if (MINGW)
    # mingw-gcc fails to link boost::thread
    add_definitions(-DBOOST_THREAD_USE_LIB)
endif()

add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
add_definitions(-DBOOST_LOG_DYN_LINK)

set(Boost_LINKAGE Boost::system Boost::thread Boost::regex Boost::log)

### Youtube-Decipher build
set(Source_Dir ${CMAKE_CURRENT_SOURCE_DIR}/src/)
set(Include_Dir ${CMAKE_CURRENT_SOURCE_DIR}/include/)

file(GLOB_RECURSE Headers ${Include_Dir}/*.h)
file(GLOB_RECURSE Sources ${Source_Dir}/*.cpp)

include_directories(
        ${Include_Dir}
        ${Boost_INCLUDE_DIRS}
)

add_library(Youtube_Decipher ${Headers} ${Sources})

target_link_libraries(Youtube_Decipher ${Boost_LINKAGE})