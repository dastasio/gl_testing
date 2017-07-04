#version 400 core
#define LIGHT_POS(x) lights[x * 2]
#define LIGHT_COLOR(x) lights[x * 2 + 1]
vec3 calcLight(int n);

in vec2 txc;
in vec3 normal;
in vec3 fragpos;
out vec4 color;

/* array of vec3 for lights. Will have structure: 
 * - vec3 position,
 * - vec3 color,
 * - and repeat 
 */
uniform vec3 lights[200];
uniform int N_LIGHTS;
uniform vec3 eye;
uniform sampler2D tex;

void main() {
	vec3 Lintensity = vec3(0.0);
	for (int i = 0; i < N_LIGHTS; ++i) {
		Lintensity += calcLight(i);
	}

	color = vec4((vec3(0.1) + Lintensity), 1.0) * vec4(1.0) + (texture(tex, txc) * 0);
}

vec3 calcLight(int n) {
	vec3 LightPosition = LIGHT_POS(n);
	vec3 LightColor = LIGHT_COLOR(n);

	/* diffuse light */
	vec3 dir = normalize(LightPosition - fragpos);
	vec3 light = LightColor * max(dot(dir, normal), 0.0);

	/* specular */
	vec3 viewdir = normalize(eye - fragpos);
	vec3 reflectedLight = reflect(-dir, normal);
	light += LightColor * pow(max(dot(viewdir, reflectedLight), 0.0), 32);

	return light;
}