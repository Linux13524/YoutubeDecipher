cmake_minimum_required(VERSION 3.8)
project(YoutubeDecipherTest CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(YoutubeDecipherTest test.cpp)
target_include_directories(YoutubeDecipherTest PRIVATE "include" ${CONAN_INCLUDE_DIRS})
target_link_libraries(YoutubeDecipherTest PRIVATE ${CONAN_LIBS})

target_compile_features(YoutubeDecipherTest PUBLIC cxx_std_11)

