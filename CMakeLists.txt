cmake_minimum_required(VERSION 3.13.0)
cmake_policy(SET CMP0091 NEW)

project(Arkanoid)

find_package(SDL3 REQUIRED)
find_package(spdlog REQUIRED)
find_package(imgui REQUIRED)
find_package(glew REQUIRED)
find_package(glm REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WIN32_EXECUTABLE ON)   # we want a Windows GUI application, not a console one

# platform detection
if(EMSCRIPTEN)
	set(TARGET_WASM ON)
	add_compile_definitions(ARK_OS_WASM)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(TARGET_WINDOWS ON)
	add_compile_definitions(ARK_OS_WIN)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
	set(TARGET_ANDROID ON)
	add_compile_definitions(ARK_OS_ANDROID)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(TARGET_LINUX ON)
	add_compile_definitions(ARK_OS_LINUX)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	set(TARGET_MACOS ON)
	add_compile_definitions(ARK_OS_MACOS)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
	set(TARGET_IOS ON)
	add_compile_definitions(ARK_OS_IOS)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "tvOS")
	set(TARGET_TVOS ON)
	add_compile_definitions(ARK_OS_TVOS)
else()
	message(FATAL_ERROR "Unsupported operating system ${CMAKE_SYSTEM_NAME}")
endif()

set(ARKANOID_HEADERS
	src/core/common.h
	src/core/logger.h
	src/core/maths.h
	src/renderer/ogl.h
	src/renderer/renderer.h
	src/renderer/shader_program.h
	src/ball.h
	src/brick.h
	src/game.h
	src/paddle.h
)

set(ARKANOID_SOURCES
	src/core/logger.cpp
	src/renderer/ogl.cpp
	src/renderer/renderer.cpp
	src/renderer/shader_program.cpp
	src/ball.cpp
	src/brick.cpp
	src/game.cpp
	src/paddle.cpp
	src/main.cpp
)

set(ARKANOID_EXTERNAL_HEADERS
	${CMAKE_BINARY_DIR}/external/imgui_impl_sdl3.h
	${CMAKE_BINARY_DIR}/external/imgui_impl_opengl3.h
)

set(ARKANOID_EXTERNAL_SOURCES
	${CMAKE_BINARY_DIR}/external/imgui_impl_sdl3.cpp
	${CMAKE_BINARY_DIR}/external/imgui_impl_opengl3.cpp
)

add_executable(Arkanoid ${ARKANOID_HEADERS} ${ARKANOID_SOURCES} ${ARKANOID_EXTERNAL_HEADERS} ${ARKANOID_EXTERNAL_SOURCES}})

target_link_libraries(Arkanoid PRIVATE SDL3::SDL3-static spdlog::spdlog GLEW::GLEW imgui::imgui glm::glm)

target_include_directories(Arkanoid PRIVATE
	"${CMAKE_BINARY_DIR}/external"
	"src"
	"src/core"
	"src/renderer"
)

if(MSVC)
	target_compile_options(Arkanoid PRIVATE /WX)
else()
	target_compile_options(Arkanoid PRIVATE -Werror -Wall -Wextra -pedantic)
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX Headers FILES ${ARKANOID_HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX Sources FILES ${ARKANOID_SOURCES})
source_group(TREE ${CMAKE_BINARY_DIR}/external PREFIX Headers/external FILES ${ARKANOID_EXTERNAL_HEADERS})
source_group(TREE ${CMAKE_BINARY_DIR}/external PREFIX Sources/external FILES ${ARKANOID_EXTERNAL_SOURCES})

