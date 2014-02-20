# Cmake build script
# ------------------

option(USE_MPI "Support for MPI" ON)

if(USE_MPI)
    add_definitions(-DUSE_MPI)
    set(CMAKE_CXX_COMPILER "mpic++")
endif()

#set(CMAKE_VERBOSE_MAKEFILE "ON")

project (ALIO CXX)
set(PROJECT_VERSION "SVN")


cmake_minimum_required (VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG" CACHE STRING
    "Flags used by the C++ compiler during ALIO release builds." FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-g -I .. -fPIC -O0 -DDEBUG" CACHE STRING
    "Flags used by the C++ compiler during ALIO debug builds." FORCE)


if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(TOOLS_SOURCE_DIR "tools")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

include_directories( "${PROJECT_SOURCE_DIR}" )

add_subdirectory("${PROJECT_SOURCE_DIR}/tools" )
add_subdirectory("${PROJECT_SOURCE_DIR}/xml"   )
add_subdirectory("${PROJECT_SOURCE_DIR}/client")
add_subdirectory("${PROJECT_SOURCE_DIR}/server")

# add_executable(server.exe server.cpp)
#add_executable(client.exe client.cpp)