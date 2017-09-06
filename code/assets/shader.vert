#version 400 core 
 
layout (location = 0) in vec3 vpos; 
layout (location = 1) in vec3 Normal; 
layout (location = 2) in vec2 tcoord; 
uniform mat4 view; 
uniform mat4 projection; 
uniform mat4 model;

out vec2 txc; 
out vec3 fragpos;
out vec4 lightSpaceFragPos;
out vec3 normal; 
 
void main() { 
  txc = tcoord;
  normal = normalize(mat3(transpose(inverse(model))) * Normal);
  fragpos = vec3(model * vec4(vpos, 1.0));
  gl_Position = projection * view * model * vec4(vpos,1.0);
}