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

uniform vec3 shadowLight_p;
uniform vec3 shadowLight_camb;
uniform vec3 shadowLight_cdiff;
uniform vec3 shadowLight_cspec;

uniform int N_PLIGHTS;
uniform int N_DLIGHTS;

uniform vec3 eye; 
uniform vec3 lightpos;
uniform Material mat;
uniform double gamma;
uniform float far_plane;

uniform samplerCube shadowMap;

uniform bool depthVisual;

void main() { 
	vec3 final = calcPointLight(-1);
	if (N_PLIGHTS > N_DLIGHTS) {
		final = vec3(0.0);
	}
	for (int i = 0; i < N_PLIGHTS; i++) {
		final += calcPointLight(i);
	}
	for (int i = 0; i < N_DLIGHTS; i++) {
		final += calcDirLight(i);
	}

	vec3 fragToLight = fragpos - lightpos;
	float closestDepth = texture(shadowMap, fragToLight).r;
 	
 	if (depthVisual)
 		color = vec4(pow(vec3(closestDepth / far_plane) / 10, vec3(1.f / gamma)), 1.0);
 	else
 		color = vec4(pow(final, vec3(1.0 / gamma)), 1.0); 
} 
 
vec3 calcPointLight(int n) {
	if (n == -1) {
		vec3 LightPosition = shadowLight_p; 
		vec3 LightDir = normalize(LightPosition - fragpos); 
		vec3 ViewDir = normalize(eye - fragpos); 
		
		/* ambient lighting */ 
		vec3 final_ambient = shadowLight_camb * texture(mat.diffuse, txc).rgb; 
		
		/* diffuse lighting */ 
		float diff_intensity = max(dot(LightDir, normal), 0.0); 
		vec3 final_diffuse = shadowLight_cdiff * diff_intensity;// * vec3(texture(mat.diffuse, txc)); 
		
		/* specular lighting */ 
		vec3 halfwayVector = normalize(LightDir + ViewDir);
		float spec_intensity = pow(max(dot(normal, halfwayVector), 0.0), 32);
		vec3 final_specular = shadowLight_cspec * spec_intensity;// * vec3(1.0); //vec3(texture(mat.specular, txc)); 
		
		float distance = length(LightPosition - fragpos);
		float attenuation = 1.0 / (distance * distance);

		/* shadow */
		float shadow = calcShadow();
		vec3 final_lighting = final_ambient + (1.0 - shadow) * (final_diffuse + final_specular);
		final_lighting *= texture(mat.diffuse, txc).rgb;
		//final_lighting *= attenuation;

		return final_lighting;
	}
	else {
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
	/* getting direction of shadow */
	vec3 fragToLight = fragpos - lightpos;
	float closestDepth = texture(shadowMap, fragToLight).r;
	closestDepth *= far_plane;

	float currentDepth = length(fragToLight);

	float bias = 0.05;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	return shadow;
}



