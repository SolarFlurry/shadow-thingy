cmake_minimum_required(VERSION 3.15)
project(ShadowThingy LANGUAGES C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_subdirectory(external/libspng)

add_executable(shadowth
	src/main.c
	src/render/block.c
	src/render/render.c
	src/render/tri/fill.c
	src/render/type/type.c
)
target_link_libraries(shadowth PRIVATE spng)
