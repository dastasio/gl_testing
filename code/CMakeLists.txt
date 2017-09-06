if (SYSWINDOWS)
	link_directories ("${PROJECT_SOURCE_DIR}/win/SDL2/lib")
endif()

add_executable (${EXEC_NAME} "main.cpp")
add_library (glad STATIC ${GLAD_SOURCE} ${GLAD_INCLUDE_DIR}/glad/glad.h ${GLAD_INCLUDE_DIR}/KHR/khrplatform.h)
add_library (gl_testing STATIC
	shaders.cpp system.cpp window.cpp camera.cpp camera_manager.cpp texture.cpp program_manager.cpp vao_manager.cpp light_manager.cpp scene.cpp mesh.cpp scene_manager.cpp 
	shaders.hpp system.hpp window.hpp camera.hpp camera_manager.hpp texture.hpp program_manager.hpp vao_manager.hpp light_manager.hpp scene.hpp mesh.hpp scene_manager.hpp oglin.hpp
	assets/shader.vert assets/shader.frag 
	assets/light.vert  assets/light.frag
	assets/shadow.vert assets/shadow.geom assets/shadow.frag)

target_link_libraries (${EXEC_NAME} gl_testing)
target_link_libraries (gl_testing glad ${OPENGL_LIBRARIES} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} opengl32 assimp)
include_directories (${PROJECT_SOURCE_DIR}/code ${PROJECT_SOURCE_DIR}/glm ${OPENGL_INCLUDE_DIR} ${SDL2_INCLUDE_DIR} ${GLAD_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR})


# c++ version
target_compile_features( ${EXEC_NAME} PRIVATE cxx_nullptr)
target_compile_features( gl_testing PRIVATE cxx_nullptr)