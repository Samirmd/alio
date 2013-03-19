# CMakeLists.txt for tools
# ------------------------

set(CMAKE_CXX_COMPILER "mpic++")
add_definitions(-D__STDC_LIMIT_MACROS)

add_library(client SHARED
 base_file_object.hpp
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
 standard_file_object.hpp
 timer_data.hpp
 timer_file_object_decorator.cpp
 timer_file_object_decorator.hpp
 timer.hpp
 timer_manager.cpp
 timer_manager.hpp
 wrapper.cpp
 ${PROJECT_SOURCE_DIR}/tools/message.cpp  
 ${PROJECT_SOURCE_DIR}/tools/message.hpp
 ${PROJECT_SOURCE_DIR}/tools/os.cpp
 ${PROJECT_SOURCE_DIR}/tools/os.hpp
 ${PROJECT_SOURCE_DIR}/tools/string_utils.cpp
 ${PROJECT_SOURCE_DIR}/tools/string_utils.hpp

)

#target_link_libraries(stdc++)

target_link_libraries(client xml)