cmake_minimum_required(VERSION 3.22)
project(Tanks)


###########################################################
# DIRECTORIES
###########################################################

set(3RD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps)


###########################################################
# 3RD PARTY LIBRARIES
###########################################################

# GLFW (window creation and input)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(${3RD_PARTY_DIR}/glfw-3.3.8)

# glad (OpenGL function loading)
add_subdirectory(${3RD_PARTY_DIR}/glad)

add_subdirectory(${3RD_PARTY_DIR}/freetype-2.13.2)
add_subdirectory(${3RD_PARTY_DIR}/assimp)



set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(Tanks)

target_compile_options(
	Tanks
	PRIVATE
	-Wall
	-O3
	# -fsanitize=address
)

target_link_options(
	Tanks
	PRIVATE
	# -fsanitize=address
)

target_sources(
	Tanks
	PRIVATE
	src/main.cpp

	src/graphics/shader.cpp
	src/graphics/mesh2D.cpp
	src/graphics/sprite.cpp
	src/graphics/texture.cpp
	src/graphics/mesh3D.cpp
	src/graphics/model.cpp

	src/core/game.cpp
	src/core/window.cpp
	src/core/camera.cpp
	src/core/main_scene.cpp
	src/core/loading_scene.cpp

	src/entities/player.cpp
	src/entities/ground.cpp
	src/entities/tank.cpp
	src/entities/building_manager.cpp
	src/entities/projectile_manager.cpp
	src/entities/enemy_manager.cpp


	src/entities/enemy.cpp

	src/utils/random_generator.cpp
	src/utils/resource_manager.cpp
	src/utils/shader_utils.cpp
	
	src/utils/colors.cpp
)

target_include_directories(
	Tanks
	PRIVATE
	src
	src/entities
	src/graphics
	src/glm
	src/stb
	src/utils
	src/core)

target_link_libraries(
	Tanks
	PRIVATE
	glad
	glfw
	freetype
	assimp)

