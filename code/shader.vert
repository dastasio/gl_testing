#version 400 core

layout (location = 0) in vec3 vpos;
layout (location = 1) in vec2 tcoord;
uniform mat4 camera;
out vec2 txc;

void main() {
	txc = tcoord;
	gl_Position = camera * vec4(vpos.xyz, 1.0);
}