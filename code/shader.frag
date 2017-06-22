#version 400 core
out vec4 color;

void main() {
	/*int c = int(mod(gl_FragCoord.x/50, 2));
	bool ymod = bool(int(mod(gl_FragCoord.y/50, 2)));
	color = ymod ? vec4(1 - vec3(c), 1.0) : vec4(c, c, c, 1.0);*/

	color = vec4(0.0, 1.0, 0.3, 1.0);
}