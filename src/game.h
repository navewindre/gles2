#pragma once

#include "gl_2d.h"

typedef struct {
  GL_DATA*    gl;
  GL_PROGRAM* gl2d;
} GAME_DATA;

GAME_DATA* game_init( GL_DATA* gl, GL_PROGRAM* gl2d );
void       game_main_loop( GAME_DATA* game );
void       game_destroy( GAME_DATA* game );
