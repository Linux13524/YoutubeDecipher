include(CMakeFindDependencyMacro)
find_dependency(Boost 1.66)
find_dependency(OpenSSL)
include("${CMAKE_CURRENT_LIST_DIR}/YoutubeDecipherTargets.cmake")

