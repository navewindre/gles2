attribute vec4 vPosition;
uniform vec4 vScreenRatio;
void main() {
	vec4 pos = vPosition;
	pos[0] -= 240.0;
	pos[1] -= 240.0;
	pos[1] *= -1.0;

	gl_Position = ( pos ) * vScreenRatio;
}
