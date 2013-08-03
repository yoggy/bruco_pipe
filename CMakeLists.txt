cmake_minimum_required(VERSION 2.8)

PROJECT(bruco_pipe)

find_library(RE2_LIBRARY
	NAMES re2
	PATHS
		/usr/lib
		/usr/local/lib)

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_C_FLAGS "-g -Wall")

include_directories(/usr/local/include)

set(src_utils log.cpp config.cpp lock.cpp string.cpp)
set(src_bruco_pipe main.cpp bruco_pipe.cpp bruco_session.cpp ${src_utils})

add_executable(bruco_pipe ${src_bruco_pipe})
add_executable(test_server test_server.c)

target_link_libraries(bruco_pipe ${RE2_LIBRARY})
INSTALL(TARGETS bruco_pipe RUNTIME DESTINATION bin)
