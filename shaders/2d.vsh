attribute vec4 vPosition;
vec4 vScreenRatio = vec4( 1.0 / 240.0, 1.0 / 240.0, 1.0, 1.0 );
void main() {
	vec4 pos = vPosition;
	pos[0] -= 240.0;
	pos[1] -= 240.0;
	pos[1] *= -1.0;

	gl_Position = ( pos ) * vScreenRatio;
}
