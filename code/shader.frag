#version 400 core
in vec2 tx_coords;

out vec4 color;

struct Material {
	sampler2D diffuse;
};

uniform Material mat;

void main() {
	color = texture(mat.diffuse, tx_coords);
}