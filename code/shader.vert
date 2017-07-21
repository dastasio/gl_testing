#version 410 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 txc;

out vec2 txcoords;

uniform mat4 model;

void main() {
	txcoords = txc;
	gl_Position = model * vec4(position, 1.0);
}