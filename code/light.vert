#version 400 core

layout (location = 0) in vec3 vpos;
uniform mat4 camera;
uniform mat4 model;

void main() {
	gl_Position = camera * model * vec4(vpos, 1.0);
}