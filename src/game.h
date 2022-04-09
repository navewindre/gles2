#pragma once

#include "gl_2d.h"

typedef struct {
  GL_PROGRAM* gl2d;
  GL_PROGRAM* gl2d_texcoord;
} GAME_SHADERS;

typedef struct {
  GL_DATA*     gl;
  GAME_SHADERS shaders;
} GAME_DATA;

GAME_DATA* game_init( GL_DATA* gl );
void       game_main_loop( GAME_DATA* game );
void       game_destroy( GAME_DATA* game );
