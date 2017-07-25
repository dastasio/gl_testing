#version 400 core
layout (location = 0) in vec3 pos;
out vec3 txcoords;
uniform mat4 view;
uniform mat4 projection;

void main() {
	txcoords = pos;
	gl_Position = (projection * vec4(mat3(view) * pos, 1.0)).xyww;
}