#version 400 core 
#define PL_POS(x)  p_lights[x * 4]
#define PL_AMB(x)  p_lights[x * 4 + 1]
#define PL_DIFF(x) p_lights[x * 4 + 2]
#define PL_SPEC(x) p_lights[x * 4 + 3]
#define DL_DIR(x)  -normalize(dir_lights[x * 4])
#define DL_AMB(x)  dir_lights[x * 4 + 1]
#define DL_DIFF(x) dir_lights[x * 4 + 2]
#define DL_SPEC(x) dir_lights[x * 4 + 3]
 
vec3 calcPointLight(int);
vec3 calcDirLight(int);
float calcShadow();
 
struct Material { 
	sampler2D diffuse; 
	sampler2D specular;
}; 
 
in vec2 txc;
in vec3 normal;
in vec3 fragpos;
in vec4 lightSpaceFragPos;
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
uniform vec3 p_lights[40];
uniform vec3 dir_lights[8];

uniform int N_PLIGHTS;
uniform int N_DLIGHTS;

uniform vec3 eye; 
uniform Material mat;
uniform double gamma;

uniform sampler2D shadowMap;

void main() { 
	vec3 final = vec3(0.0);
	if (N_PLIGHTS > N_DLIGHTS) {
		final = vec3(0.0);
	}
	for (int i = 0; i < N_PLIGHTS; i++) {
		final += calcPointLight(i);
	}
	for (int i = 0; i < N_DLIGHTS; i++) {
		final += calcDirLight(i);
	}
 
 	color = vec4(pow(final, vec3(1.0 / gamma)), 1.0); 
} 
 
vec3 calcPointLight(int n) { 
	vec3 LightPosition = PL_POS(n); 
	vec3 LightDir = normalize(LightPosition - fragpos); 
	vec3 ViewDir = normalize(eye - fragpos); 
	
	/* ambient lighting */ 
	vec3 final_ambient = PL_AMB(n) * vec3(texture(mat.diffuse, txc)); 
	
	/* diffuse lighting */ 
	float diff_intensity = max(dot(normal, LightDir), 0.0); 
	vec3 final_diffuse = PL_DIFF(n) * diff_intensity * vec3(texture(mat.diffuse, txc)); 
	
	/* specular lighting */ 
	vec3 halfwayVector = normalize(LightDir + ViewDir);
	float spec_intensity = pow(max(dot(normal, halfwayVector), 0.0), 32);
	vec3 final_specular = PL_SPEC(n) * spec_intensity * vec3(1.0); //vec3(texture(mat.specular, txc)); 
	
	float distance = length(LightPosition - fragpos);
	float attenuation = 1.0 / (distance * distance);//(1.0 + (0.09 * distance) + (0.032 * distance * distance));
	return attenuation * (final_ambient + final_diffuse + final_specular);
}

vec3 calcDirLight(int n) {
	vec3 LightDir = DL_DIR(n);
	vec3 ViewDir = normalize(eye - fragpos);

	/* ambient */
	vec3 final_ambient = 0.3 * DL_AMB(n) * vec3(texture(mat.diffuse, txc));

	/* diffuse */
	float diff_intensity = max(dot(normal, LightDir), 0.0);
	vec3 final_diffuse = DL_DIFF(n) * diff_intensity * vec3(texture(mat.diffuse, txc));

	/* specular */
	vec3 halfwayVector = normalize(LightDir + ViewDir);
	float spec_intensity = pow(max(dot(normal, halfwayVector), 0.0), 64);
	vec3 final_specular = DL_SPEC(n) * spec_intensity * vec3(1.0);

	/* shadow */
	float shadow = calcShadow();
	vec3 final_lighting = final_ambient + (1.0 - shadow) * (final_diffuse + final_specular);

	return final_lighting;
}

float calcShadow() {
	// perspective division
	vec3 projCoord = lightSpaceFragPos.xyz / lightSpaceFragPos.w;
	projCoord = projCoord * 0.5 + 0.5;

	/*float closestDepth = texture(shadowMap, projCoord.xy).r; */
	float currentDepth = projCoord.z;
	float bias = max(0.05 * (1.0 - dot(normal, DL_DIR(0))), 0.001);
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			float tmpDepth = texture(shadowMap, projCoord.xy + texelSize * vec2(x, y)).r;
			shadow += currentDepth - bias > tmpDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	if (projCoord.z > 1.0)
		return 0.0;

	return shadow;
}



