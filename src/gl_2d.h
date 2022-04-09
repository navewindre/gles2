#pragma once
#include "gl.h"


GL_PROGRAM* gl_2d_init( GL_DATA* gl, VEC2 screensize, const char* shadername );

void gl_polygon( GL_PROGRAM* gl2d, VERTEX* vertices, U32 vertices_count );
void gl_2d_line( GL_PROGRAM* gl2d, VEC2 start, VEC2 end, CLR col );
void gl_2d_rect( GL_PROGRAM* gl2d, VEC2 origin, VEC2 dimensions, CLR col );
void gl_2d_frect( GL_PROGRAM* gl2d, VEC2 origin, VEC2 dimensions, CLR col );
void gl_2d_circle( GL_PROGRAM* gl2d, VEC2 origin, F32 radius, CLR col );
void gl_2d_fcircle( GL_PROGRAM* gl2d, VEC2 origin, F32 radius, CLR col );
