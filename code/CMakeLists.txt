if (SYSWINDOWS)
	link_directories ("${PROJECT_SOURCE_DIR}/win/glew/lib")
	link_directories ("${PROJECT_SOURCE_DIR}/win/SDL2/lib")
endif()

add_executable (${EXEC_NAME} "main.cpp")
add_library (gl_testing SHARED
	shaders.cpp system.cpp window.cpp camera.cpp camera_manager.cpp texture.cpp program_manager.cpp vao_manager.cpp light_manager.cpp scene.cpp mesh.cpp scene_manager.cpp 
	shaders.hpp system.hpp window.hpp camera.hpp camera_manager.hpp texture.hpp program_manager.hpp vao_manager.hpp light_manager.hpp scene.hpp mesh.hpp scene_manager.hpp oglin.hpp
	shader.vert shader.frag shader.geom light.vert light.frag)


target_link_libraries (${EXEC_NAME} gl_testing)
target_link_libraries (gl_testing ${OPENGL_LIBRARIES} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${GLEW_LIBRARY} assimp)
include_directories (${PROJECT_SOURCE_DIR}/code ${PROJECT_SOURCE_DIR}/glm ${OPENGL_INCLUDE_DIR} ${SDL2_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR})


# c++ version
target_compile_features( ${EXEC_NAME} PRIVATE cxx_nullptr)
target_compile_features( gl_testing PRIVATE cxx_nullptr)