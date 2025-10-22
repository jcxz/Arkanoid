find_package(SDL3 REQUIRED)
find_package(spdlog REQUIRED)

set(ARKANOID_HEADERS)
set(ARKANOID_SOURCES main.cpp)

add_executable(Arkanoid ${ARKANOID_HEADERS} ${ARKANOID_SOURCES})

target_link_libraries(Arkanoid PRIVATE SDL3::SDL3-static spdlog::spdlog)

if(MSVC)
	target_compile_options(Arkanoid PRIVATE /WX)
else()
	target_compile_options(Arkanoid PRIVATE -Werror -Wall -Wextra -pedantic)
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX Headers FILES ${ARKANOID_HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX Sources FILES ${ARKANOID_SOURCES})
