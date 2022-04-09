precision mediump float;
uniform vec4 vColor;
varying vec2 vTexCoord;
void main() {
  gl_FragColor = vec4( vTexCoord[0], vTexCoord[1], 1.0, 1.0 );
}
