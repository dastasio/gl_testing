#version 400 core

in vec2 txcoords;

out vec4 color;

uniform sampler2D tx;

/*const float offset = 1.0 / 300.0;
float kernel[9] = float[9](
	-1, -1, -1,
    -1,  8, -1,
    -1, -1, -1
);*/
void main() {
	/*vec2 offsets[9] = vec2[](
		vec2(-offset,	 offset),
		vec2( 0.0,		 offset),
		vec2( offset, 	 offset),
		vec2(-offset, 	 0.0),
		vec2( 0.0, 		 0.0),
		vec2( offset, 	 0.0),
		vec2(-offset,	-offset),
		vec2( 0.0,		-offset),
		vec2( offset,	-offset)
	);

	vec3 final = vec3(0.0);
	final += vec3(texture(tx, txcoords.st + offsets[0])) * kernel[0];
	final += vec3(texture(tx, txcoords.st + offsets[1])) * kernel[1];
	final += vec3(texture(tx, txcoords.st + offsets[2])) * kernel[2];
	final += vec3(texture(tx, txcoords.st + offsets[3])) * kernel[3];
	final += vec3(texture(tx, txcoords.st + offsets[4])) * kernel[4];
	final += vec3(texture(tx, txcoords.st + offsets[5])) * kernel[5];
	final += vec3(texture(tx, txcoords.st + offsets[6])) * kernel[6];
	final += vec3(texture(tx, txcoords.st + offsets[7])) * kernel[7];
	final += vec3(texture(tx, txcoords.st + offsets[8])) * kernel[8];*/

	color = texture(tx, txcoords);
}