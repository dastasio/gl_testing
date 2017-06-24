#version 400 core

in vec2 txc;
out vec4 color;

uniform sampler2D tex;

void main() {
	/*int c = int(mod(gl_FragCoord.x/50, 2));
	bool ymod = bool(int(mod(gl_FragCoord.y/50, 2)));
	color = ymod ? vec4(1 - vec3(c), 1.0) : vec4(c, c, c, 1.0);*/

	color = texture(tex, txc);
}