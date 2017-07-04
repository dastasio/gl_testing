#version 400 core

layout (location = 0) in vec3 vpos;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 tcoord;
uniform mat4 camera;
uniform mat4 model;
out vec2 txc;
out vec3 fragpos;
out vec3 normal;

void main() {
	txc = tcoord;
	normal = normalize(Normal);
	fragpos = vec3(model * vec4(vpos, 1.0));
	gl_Position = camera * vec4(fragpos,1.0);
}