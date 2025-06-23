cmake_minimum_required(VERSION 4.0.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(todosqlite)

include_directories(${CMAKE_SOURCE_DIR}/external)

find_package(OpenSSL REQUIRED)

add_executable(todosqlite
	external/sqlite/sqlite3.c
	src/app/main.cpp
	src/app/Menu_item.cpp
	src/core/Priority.cpp
	src/core/Task.cpp
	src/utils/Encrypt.cpp
	src/utils/Input.cpp
)

target_link_libraries(todosqlite
	PRIVATE
	dl
	OpenSSL::SSL
	pthread
)
