#pragma once

#include <SDL.h>
#include <SDL_opengles2.h>
#include <GLES2/gl2.h>

#include "allocator.h"
#include "math.h"

typedef struct {
  F32 r;
  F32 g;
  F32 b;
  F32 a;
} CLR;

typedef struct {
  VEC3 pos;
} VERTEX;

typedef struct {
  const char* code;
  I32         type;
  U32         id;
  U8          compiled;
} GL_SHADER;

typedef struct {
  const char* name;
  GL_SHADER   fsh;
  GL_SHADER   vsh;

  U32 id;
  U32 vbuffer;
} GL_PROGRAM;

typedef struct {
  SDL_Window*   window;
  SDL_GLContext ctx;
  SDL_Renderer* renderer;
  
  LIST_ITEM* programs;
  U32 program_count;
} GL_DATA;

GL_DATA*    gl_init();
void        gl_destroy( GL_DATA* gl );
U8          gl_beginframe( GL_DATA* gl );
U8          gl_endframe( GL_DATA* gl );
U8          gl_shader_compile( GL_DATA* gl, GL_SHADER* shader );
void        gl_shader_destroy( GL_DATA* gl, GL_SHADER* shader );
GL_PROGRAM* gl_program_create( GL_DATA* gl, const char* name );
U8          gl_program_compile( GL_DATA* gl, GL_PROGRAM* program );
void        gl_program_destroy( GL_DATA* gl, GL_PROGRAM* program );
