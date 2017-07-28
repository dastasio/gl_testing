#version 400 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform double anim_time;

in VS_OUT {
	vec3 normal;
	vec2 coords;
} gs_in[];
out vec2 tx_coords;

vec4 explode(vec4 original, vec3 n) {
	return original + vec4((anim_time * n), 1.0);
}

vec3 GenNormal() {
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

void main() {;
	vec3 N = GenNormal();

	gl_Position = explode(gl_in[0].gl_Position, N);
	tx_coords = gs_in[0].coords;
	EmitVertex();

	gl_Position = explode(gl_in[1].gl_Position, N);
	tx_coords = gs_in[1].coords;
	EmitVertex();

	gl_Position = explode(gl_in[2].gl_Position, N);
	tx_coords = gs_in[2].coords;
	EmitVertex();

	EndPrimitive();
}