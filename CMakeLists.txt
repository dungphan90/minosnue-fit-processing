cmake_minimum_required (VERSION 3.17)
project (NueRedo)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
set (CMAKE_BUILD_TYPE "RELEASE")

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake.
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT.
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

# Locates the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS).
find_package(ROOT REQUIRED)


# Defines useful ROOT functions and ROOT macros (e.g. ROOT_GENERATE_DICTIONARY).
include(${ROOT_USE_FILE})

add_executable(NueRedo main.cpp)
target_link_libraries(NueRedo ${ROOT_LIBRARIES} Core)
