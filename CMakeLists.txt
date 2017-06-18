cmake_minimum_required (VERSION 3.1)

project (gl_testing)
set (EXEC_NAME gl_test1)

set (EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set (LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})
set (MY_LIBS "${PROJECT_SOURCE_DIR}/code")
set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmodules")

message ("system name: " ${CMAKE_SYSTEM})
message ("system processor: " ${CMAKE_SYSTEM_PROCESSOR})

# setting external libs' variables
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set (SYSWINDOWS 1)
	# manually setting include/library variables
	set (GLEW_LIBRARY "glew32s opengl32")
	set (GLEW_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/win/glew/include)
	set (SDL2_LIBRARY "SDL2 SDL2_image")
	set (SDL2_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/win/SDL2/include)
	message ("detected Windows system")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set (SYSLINUX 1)
	set (SYSUNIX 1)

	find_package (OPNEGL REQUIRED)
	find_package (GLEW REQUIRED)
	find_package (SDL2 REQUIRED)
	find_package (SDL2_image REQUIRED)

	message ("detected Linux system")
else()
	set (SYSMAC 1)
	set (SYSUNIX 1)

	find_package (OPENGL REQUIRED)
	# GLEW variables are null
	find_package (SDL2 REQUIRED)
	find_package (SDL2_image REQUIRED)

	message ("detected macOS")
endif()


set (GLM_STATIC_LIBRARY_ENABLE ON CACHE BOOL "" FORCE)
#set (ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#set (BUILD_ASSIMP_SAMPLES OFF CACHE BOOL "" FORCE)


add_subdirectory(${MY_LIBS})






