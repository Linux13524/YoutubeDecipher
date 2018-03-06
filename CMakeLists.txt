cmake_minimum_required(VERSION 3.9)
project(Youtube_Decipher)

set(CMAKE_CXX_STANDARD 11)

### OPENSSL - static - lib*.a
set(OPENSSL_USE_STATIC_LIBS ON)
find_package(OpenSSL REQUIRED)

set(OpenSSL_LINKAGE OpenSSL::SSL OpenSSL::Crypto)


### BOOST - static - libboost_*-mt.a
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system regex REQUIRED)

set(CMAKE_USE_LIBSSH2 ON)
set(CMAKE_USE_OPENSSL ON)

set(Boost_LINKAGE Boost::system Boost::regex )

### Youtube-Decipher build
file(GLOB_RECURSE Headers include/*.h include/*.hpp)
file(GLOB_RECURSE Sources src/*.cpp)

include_directories(
        include
        include/utils/json
        ${Boost_INCLUDE_DIRS}
)

add_library(Youtube_Decipher STATIC ${Headers} ${Sources})

if(MINGW)
    target_link_libraries(Youtube_Decipher PRIVATE  ${Boost_LINKAGE} ${OpenSSL_LINKAGE} ws2_32)
else()
    target_link_libraries(Youtube_Decipher PRIVATE  ${Boost_LINKAGE} ${OpenSSL_LINKAGE})
endif()