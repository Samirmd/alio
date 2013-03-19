# CMakeLists.txt for tools
# ------------------------

set(CMAKE_CXX_COMPILER "mpic++")
add_definitions(-D__STDC_LIMIT_MACROS)

add_executable(server
 server.cpp
)

target_link_libraries (server tools)
