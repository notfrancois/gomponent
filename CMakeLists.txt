cmake_minimum_required(VERSION 3.19)
project(Go LANGUAGES C CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(sdk)
add_subdirectory(network)

include_directories(
	.
	events
)

add_library(${PROJECT_NAME} SHARED
	main.cpp
	Gomponent.cpp
	Gamemode.cpp
	Plugin.cpp
	
	"api/api.hpp"
	"api/Actor.cpp"
	"api/Checkpoint.cpp"
	"api/Class.cpp"
	"api/Config.cpp"
	"api/Console.cpp"
	"api/CustomModel.cpp"
	"api/GangZone.cpp"
	"api/Menu.cpp"
	"api/Object.cpp"
	"api/Pickup.cpp"
	"api/Player.cpp"
	"api/Server.cpp"
	"api/TextDraw.cpp"
	"api/TextLabel.cpp"
	"api/Vehicle.cpp"
	"api/api.cpp"
)

set_target_properties(Go PROPERTIES OUTPUT_NAME "Go")
set_target_properties(Go PROPERTIES PREFIX "")

target_link_libraries(${PROJECT_NAME} PRIVATE
    OMP-SDK
)

if(TARGET_BUILD_ARCH MATCHES "(i[3-6]86|x86)" AND NOT TARGET_BUILD_ARCH MATCHES "(x64|x86_64|amd64)")
    set(CMAKE_C_FLAGS "-m32 ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "-m32 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_SIZEOF_VOID_P 4)
endif ()
