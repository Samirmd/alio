# CMakeLists.txt for tools
# ------------------------

add_definitions(-D__STDC_LIMIT_MACROS)

add_executable(server
 server.cpp
)

target_link_libraries (server tools)
