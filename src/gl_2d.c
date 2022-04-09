#include "gl_2d.h"
#include "allocator.h"
#include "math.h"
#include <GLES2/gl2.h>

GL_PROGRAM* gl_2d_init( GL_DATA* gl, VEC2 screensize ) {
  GL_PROGRAM* program = gl_program_create( gl, "2d" );
  gl_program_compile( gl, program );

  F32 screen_ratio[] = { 
    2.f / screensize.x, 
    2.f / screensize.y,
    1.f, 
    1.f 
  };
 
  glUseProgram( program->id );

  I32 ratio_location = glGetUniformLocation( program->id, "vScreenRatio" );
  glUniform4fv( ratio_location, 1, &screen_ratio[0] );

  return program;
}


void gl_2d_line( GL_PROGRAM* gl2d, VEC2 start, VEC2 end, CLR col ) {
  static const U16 order[] = { 0, 1 };

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

void gl_2d_rect( GL_PROGRAM* gl2d, VEC2 origin, VEC2 dim, CLR col ) {
  static const U16 order[] = { 0, 1, 2, 3, 4 };
  
  glUseProgram( gl2d->id );

  static const F32 color_ratio = 1.f / 255.f;
  F32 fcol[] = {
    col.r * color_ratio,
    col.g * color_ratio,
    col.b * color_ratio,
    col.a * color_ratio
  };

  VERTEX vertices[] = {
    { origin.x        , origin.y        , 0.f },
    { origin.x + dim.x, origin.y        , 0.f },
    { origin.x + dim.x, origin.y + dim.y, 0.f },
    { origin.x        , origin.y + dim.y, 0.f },
    { origin.x        , origin.y        , 0.f },
  };

 
  I32 position = glGetAttribLocation( gl2d->id, "vPosition" );
  glEnableVertexAttribArray( position );
  glVertexAttribPointer( position, 3, GL_FLOAT, 0, 12, &vertices[0] );

  I32 color = glGetUniformLocation( gl2d->id, "vColor" );
  glUniform4fv( color, 1, fcol ); 

  glDrawElements( GL_LINE_STRIP, 5, GL_UNSIGNED_SHORT, order ); 
}

void gl_2d_frect( GL_PROGRAM* gl2d, VEC2 origin, VEC2 dim, CLR col ) {
  static const U16 order[] = { 0, 1, 2, 3 };

  glUseProgram( gl2d->id );

  static const F32 color_ratio = 1.f / 255.f;
  F32 fcol[] = {
    col.r * color_ratio,
    col.g * color_ratio,
    col.b * color_ratio,
    col.a * color_ratio
  };

  VERTEX vertices[] = {
    { origin.x,         origin.y        , 0.f },
    { origin.x + dim.x, origin.y        , 0.f },
    { origin.x,         origin.y + dim.y, 0.f },
    { origin.x + dim.x, origin.y + dim.y, 0.f }
  };

  I32 position = glGetAttribLocation( gl2d->id, "vPosition" );
  glEnableVertexAttribArray( position );
  glVertexAttribPointer( position, 3, GL_FLOAT, 0, 12, &vertices[0] );

  I32 color = glGetUniformLocation( gl2d->id, "vColor" );
  glUniform4fv( color, 1, fcol ); 

  glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, order ); 
}

void gl_2d_circle( GL_PROGRAM* gl2d, VEC2 origin, F32 radius, CLR col ) {
  static U16*      order = 0;
  static const U32 res = 48;
  static const F32 step = 360.f / (F32)res;

  glUseProgram( gl2d->id );

  if( !order ) {
    order = array_alloc( sizeof( U16 ), res + 1 );
    for( U32 i = 0; i < res + 1; ++i )
      order[i] = i;
  }

  static const F32 color_ratio = 1.f / 255.f;
  F32 fcol[] = {
    col.r * color_ratio, 
    col.g * color_ratio, 
    col.b * color_ratio, 
    col.a * color_ratio  
  };
  
  VERTEX vertices[res + 1];
  for( U32 i = 0; i < res + 1; ++i ) {
    VEC2 offset = m_radial_offset( step * ( i == res? 0 : i ), radius );
    
    vertices[i] = (VERTEX){
      origin.x + offset.x,
      origin.y + offset.y,
      0.f
    };
  }

  I32 position = glGetAttribLocation( gl2d->id, "vPosition" );
  glEnableVertexAttribArray( position );
  glVertexAttribPointer( position, 3, GL_FLOAT, 0, 12, &vertices[0] );

  I32 color = glGetUniformLocation( gl2d->id, "vColor" );
  glUniform4fv( color, 1, fcol ); 

  glDrawElements( GL_LINE_STRIP, res + 1, GL_UNSIGNED_SHORT, order ); 
}

void gl_2d_fcircle( GL_PROGRAM* gl2d, VEC2 origin, F32 radius, CLR col ) {
  static U16*      order = 0;
  static const U32 res = 48;
  static const F32 step = 360.f / (F32)res;

  glUseProgram( gl2d->id );

  if( !order ) {
    order = array_alloc( sizeof( U16 ), res * 2 );
    for( U32 i = 0; i < res * 2; ++i )
      order[i] = i;
  }

  static const F32 color_ratio = 1.f / 255.f;
  F32 fcol[] = {
    col.r * color_ratio, 
    col.g * color_ratio, 
    col.b * color_ratio, 
    col.a * color_ratio 
  };
  
  VERTEX vertices[res * 2];
  memcpy( &vertices[0], &origin, sizeof( VEC2 ) );
  for( U32 i = 0; i < res * 2; i += 2 ) {
    VEC2 offset = m_radial_offset( step * ( i == res? 0 : i ), radius );
    vertices[i] = (VERTEX){
      origin.x,
      origin.y,
      0.f
    };

    vertices[i + 1] = (VERTEX){
      origin.x + offset.x,
      origin.y + offset.y,
      0.f
    };
  };

  I32 position = glGetAttribLocation( gl2d->id, "vPosition" );
  glEnableVertexAttribArray( position );
  glVertexAttribPointer( position, 3, GL_FLOAT, 0, 12, &vertices[0] );

  I32 color = glGetUniformLocation( gl2d->id, "vColor" );
  glUniform4fv( color, 1, fcol ); 

  glDrawElements( GL_TRIANGLE_STRIP, res * 2, GL_UNSIGNED_SHORT, order ); 
}
