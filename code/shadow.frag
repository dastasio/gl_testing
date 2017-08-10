#version 400 core
in vec4 fragpos;

uniform vec3 lightPos;
uniform float far_plane;

void main() {
	/* distance fragment-light */
	float lightDist = length(fragpos.xyz - lightPos);

	/* mapping to [0;1] range */
	lightDist /= far_plane;

	gl_FragDepth = lightDist;
}