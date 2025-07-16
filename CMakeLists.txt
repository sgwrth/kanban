cmake_minimum_required(VERSION 4.0.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(todosqlite)

include_directories(${CMAKE_SOURCE_DIR}/external)

find_package(OpenSSL REQUIRED)

add_executable(todosqlite
	external/sqlite/sqlite3.c
	src/app/main.cpp
	src/app/Credentials.cpp
	src/app/Menu_item.cpp
	src/core/Task.cpp
	src/utils/Crypto.cpp
	src/utils/DB.cpp
	src/utils/Input.cpp
	src/utils/Menu.cpp
	src/utils/Output.cpp
	src/utils/QueryParam.cpp
	src/utils/QueryStmt.cpp
	src/utils/Sql.cpp
	src/utils/System.cpp
	src/utils/Text.cpp
)

target_link_libraries(todosqlite
	PRIVATE
	cryptopp
	dl
	OpenSSL::SSL
	pthread
)

# Googletest
include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()
add_executable(tests
  tests/input_test.cpp
)
target_link_libraries(tests
  GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(tests)
