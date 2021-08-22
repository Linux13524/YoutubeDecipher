include(CMakeFindDependencyMacro)
find_dependency(Boost 1.69)
find_dependency(OpenSSL)
include("${CMAKE_CURRENT_LIST_DIR}/YoutubeDecipher-targets.cmake")

