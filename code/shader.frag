#version 400 core
in vec3 geom_col;

out vec4 color;

void main() {
	color = vec4(geom_col, 1.0);
}