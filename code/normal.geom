#version 400 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in vec3 normal[];

void GenLine(vec4 position, vec3 n) {
	gl_Position = position;
	EmitVertex();

	gl_Position = position + vec4(0.2 * n, 0.0);
	EmitVertex();
	EndPrimitive();
}

vec3 GenNormal() {
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

void main() {;
	GenLine(gl_in[0].gl_Position, normal[0]);
	GenLine(gl_in[1].gl_Position, normal[1]);
	GenLine(gl_in[2].gl_Position, normal[2]);
}