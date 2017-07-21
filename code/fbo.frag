#version 400 core
#define L_POS(x) lights[x * 4]
#define L_AMB(x) lights[x * 4 + 1]
#define L_DIFF(x) lights[x * 4 + 2]
#define L_SPEC(x) lights[x * 4 + 3]

vec3 calcLight(int n);

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shineFactor;
};

in vec2 txc;
in vec3 normal;
in vec3 fragpos;
out vec4 color;

/* vec3 array to describe lights; has structure:
 [0] position,
 [1] ambient color,
 [2] diffuse color,
 [3] specular color,
 [4] position of second light,
 .
 .
 .
 [3n] specular color of last light
 */
uniform vec3 lights[40];
uniform int N_LIGHTS;
uniform vec3 eye;
uniform Material mat;

void main() {
	vec3 lighting = vec3(0.0);
	for (int i = 0; i < N_LIGHTS; ++i) {
		lighting += calcLight(i);
	}

	color = texture(mat.diffuse, txc);
}

vec3 calcLight(int n) {
	vec3 LightPosition = L_POS(n);
	vec3 LightDir = normalize(LightPosition - fragpos);
	vec3 ViewDir = normalize(eye - fragpos);

	/* ambient lighting */
	vec3 final_ambient = L_AMB(n) * vec3(texture(mat.diffuse, txc));

	/* diffuse lighting */
	float diff_intensity = max(dot(normal, LightDir), 0.0);
	vec3 final_diffuse = L_DIFF(n) * diff_intensity * vec3(texture(mat.diffuse, txc));

	/* specular lighting */
	vec3 ReflectedLight = reflect(-LightDir, normal);
	float spec_intensity = pow(max(dot(ViewDir, ReflectedLight), 0.0), mat.shineFactor);
	vec3 final_specular = L_SPEC(n) * spec_intensity * vec3(texture(mat.specular, txc));

	return final_ambient + final_diffuse + final_specular;
}
