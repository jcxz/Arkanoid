cmake_minimum_required(VERSION 3.13.0)
cmake_policy(SET CMP0091 NEW)

project(Arkanoid)

find_package(SDL3 REQUIRED)
find_package(spdlog REQUIRED)
find_package(imgui REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ARKANOID_HEADERS
	external/imgui_impl_sdl3.h
	external/imgui_impl_sdlrenderer3.h

	src/ball.h
	src/brick.h
	src/game.h
	src/paddle.h
)

set(ARKANOID_SOURCES
	external/imgui_impl_sdl3.cpp
	external/imgui_impl_sdlrenderer3.cpp

	src/ball.cpp
	src/brick.cpp
	src/game.cpp
	src/paddle.cpp
	src/main.cpp
)

add_executable(Arkanoid ${ARKANOID_HEADERS} ${ARKANOID_SOURCES})

target_link_libraries(Arkanoid PRIVATE SDL3::SDL3-static spdlog::spdlog imgui::imgui)

if(MSVC)
	target_compile_options(Arkanoid PRIVATE /WX)
else()
	target_compile_options(Arkanoid PRIVATE -Werror -Wall -Wextra -pedantic)
endif()

#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX Headers FILES ${ARKANOID_HEADERS})
#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX Sources FILES ${ARKANOID_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX Headers FILES ${ARKANOID_HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX Sources FILES ${ARKANOID_SOURCES})
