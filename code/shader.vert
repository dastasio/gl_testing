#version 410 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 txc;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out VS_OUT {
	vec3 normal;
	vec2 coords;
} vs_out;

void main() {
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vs_out.normal = normalize(norm);//normalize(mat3(inverse(model)) * norm);
	vs_out.coords = txc;
}