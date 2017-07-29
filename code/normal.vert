#version 410 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 txc;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 normal;

void main() {
	gl_Position = projection * view * model * vec4(pos, 1.0);
	mat3 matrix = mat3(transpose(inverse(view * model)));
	normal = normalize(vec3(projection * vec4(matrix * norm, 1.0)));
}