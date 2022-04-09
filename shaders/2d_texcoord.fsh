precision mediump float;
uniform vec4 vColor;
varying vec2 vTexCoord;

float pow( float num ) {
  return num * num;
}

vec2 sqr_imaginary( vec2 num ) {
  return vec2(
    pow( num[0], 2.0 ) - pow( num[1], 2.0 ),
    2.0 * num[0] * num[1]
  );
}

float mandelbrot( vec2 coord ) {
  vec2 z = vec2( 0.0, 0.0 );
  for( int i = 0; i < 500; ++i ) {
    z = sqr_imaginary( z ) + coord;
    if( dot( z, z ) > 4.0 )
      return float( i ) / 500.0;
  }

  return 500.0;
}

void main() {
  vec2 zoom_location = vec2( -0.3757117, 0.301642 ); 

  float c = mandelbrot( ( vTexCoord ) * vColor[0] + zoom_location );
  gl_FragColor = vec4( c, c * 0.2, c * c, vColor[3] );
}
