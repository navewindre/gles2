attribute vec4 vPosition;
attribute vec2 inTexCoord;

uniform vec4 vScreenRatio;
varying vec2 vTexCoord;

void main() {
	vec4 pos = vPosition;
	pos[0] -= 240.0;
	pos[1] -= 240.0;
	pos[1] *= -1.0;

	gl_Position = ( pos ) * vScreenRatio;
	vTexCoord = inTexCoord;
}
