#version 400 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 col;
layout (location = 2) in vec2 translation;

out vec3 color;

void main() {
	vec2 size = vec2(gl_InstanceID / 100.0);
	gl_Position = vec4(size * pos + translation, 0.0, 1.0);
	color = col;
}