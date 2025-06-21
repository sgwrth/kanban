cmake_minimum_required(VERSION 4.0.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_executable(helloworld
	src/app/main.cpp
	src/app/Menue_item.cpp
	src/core/Task.cpp
)
