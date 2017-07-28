#version 400 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
	vec3 color;
} gs_in[];

out vec3 geom_col;

void main() {
	geom_col = gs_in[0].color;

	gl_Position = gl_in[0].gl_Position + vec4(-0.2, -0.2, 0.0, 0.0);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4( 0.2, -0.2, 0.0, 0.0);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(-0.2,  0.2, 0.0, 0.0);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4( 0.2, 0.2, 0.0, 0.0);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4( 0.0, 0.4, 0.0, 0.0);
	geom_col = vec3(1.0);
	EmitVertex();

	EndPrimitive();
}