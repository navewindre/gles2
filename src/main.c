#include <emscripten.h>
#include <emscripten/html5.h>

#include "game.h"


#ifdef EMSCRIPTEN 
void emscripten_canvas_setup() {
  emscripten_set_canvas_element_size( "#canvas", 480, 480 );
  EmscriptenWebGLContextAttributes attr;
  emscripten_webgl_init_context_attributes( &attr );
  attr.alpha = attr.depth = attr.stencil = attr.antialias = attr.preserveDrawingBuffer = 
  attr.failIfMajorPerformanceCaveat = 0;

  attr.enableExtensionsByDefault = 1;
  attr.premultipliedAlpha = 0;
  attr.majorVersion = 2;
  attr.minorVersion = 0;
  EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attr);
  emscripten_webgl_make_context_current(ctx);
}
#endif

int main() {
#ifdef EMSCRIPTEN 
  emscripten_canvas_setup();
#endif

  GL_DATA* gl = gl_init();
  GL_PROGRAM* gl2d = gl_2d_init( gl, (VEC2){ 480.f, 480.f } );
  GAME_DATA* game = game_init( gl, gl2d );

  game_main_loop( game );

  game_destroy( game );
  gl_destroy( gl );
}
