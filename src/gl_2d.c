#include "gl_2d.h"
#include <GLES2/gl2.h>

GL_PROGRAM* gl_2d_init( GL_DATA* gl, VEC2 screensize ) {
  GL_PROGRAM* program = gl_program_create( gl, "2d" );
  gl_program_compile( gl, program );
  /*
  F32 screen_ratio[] = { 1.f / screensize.x, 1.f / screensize.y, 1.f, 1.f };
  I32 ratio_location = glGetUniformLocation( program->id, "vScreenPosition" );
  glUniform4fv( ratio_location, 1, &screen_ratio[0] );
  */
  return program;
}


void gl_2d_line( GL_PROGRAM *gl2d, VEC2 start, VEC2 end, CLR col ) {
  static U16    order[2] = { 0, 1 };

  glUseProgram( gl2d->id );
  
  static const F32 color_ratio = 1.f / 255.f;
  F32 fcol[] = {
    col.r * color_ratio, 
    col.g * color_ratio, 
    col.b * color_ratio, 
    col.a * color_ratio  
  };
  
  VERTEX vertices[] = {
    { start.x, start.y, 0.f },
    { end.x,   end.y,   0.f },
  };

  I32 position = glGetAttribLocation( gl2d->id, "vPosition" );

  glEnableVertexAttribArray( position );
  glVertexAttribPointer( position, 3, GL_FLOAT, 0, 12, &vertices[0] );

  I32 color = glGetUniformLocation( gl2d->id, "vColor" );
  glUniform4fv( color, 1, fcol ); 

  glDrawElements( GL_LINES, 2, GL_UNSIGNED_SHORT, order );
}
