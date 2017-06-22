#version 400 core

layout (location = 0) in vec3 vpos;
uniform mat4 camera;

void main() {
	gl_Position = camera * vec4(vpos.xyz, 1.0);
}