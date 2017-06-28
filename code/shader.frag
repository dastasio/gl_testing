#version 400 core
#define N_LIGHTS 1

struct Light {
	vec3 pos;
};

in vec2 txc;
in vec3 normal;
in vec3 fragpos;
out vec4 color;

uniform Light lights[N_LIGHTS];
uniform vec3 eye;
uniform sampler2D tex;

vec3 calcLight(int n);

void main() {
	vec3 Lintensity = vec3(0.0);
	for (int i = 0; i < N_LIGHTS; ++i) {
		Lintensity += calcLight(i);
	}

	color = vec4((vec3(0.1) + Lintensity), 1.0) * texture(tex, txc);
}

vec3 calcLight(int n) {
	/* diffuse light */
	vec3 dir = normalize(lights[n].pos - fragpos);

	vec3 light = vec3(1.0) * max(dot(dir, normal), 0.0);

	/* specular */
	vec3 viewdir = normalize(eye - fragpos);
	light += 0.5 * pow(max(dot(viewdir, reflect(-dir, normal)), 0.0), 32);

	return light;
}