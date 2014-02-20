# CMakeLists.txt for tools
# ------------------------

add_definitions(-D__STDC_LIMIT_MACROS)

add_library(client SHARED
 base_file_object.hpp
 buffered.cpp
 buffered.hpp
 config.cpp
 config.hpp
 debug_file_object_decorator.hpp
 file_object_info.cpp
 file_object_info.hpp
 i_file_object_decorator.hpp
 i_file_object.hpp
 init.cpp
 mirror.hpp
 null_file_object.hpp
 remote.cpp
 remote.hpp
 request.cpp
 request.hpp
 standard_file_object.hpp
 timer_data.hpp
 timer_file_object_decorator.cpp
 timer_file_object_decorator.hpp
 timer.hpp
 timer_manager.cpp
 timer_manager.hpp
 wrapper.cpp
)

target_link_libraries(client tools xml)
