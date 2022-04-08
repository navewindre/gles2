#pragma once

#include <SDL.h>
#include "typedef.h"

typedef struct {
  U8 r;
  U8 g;
  U8 b;
  U8 a;
} CLR;

typedef struct {
  SDL_Window*   window;
  SDL_Renderer* renderer;
} SDL_DATA;

SDL_DATA* sdl_init();
void      sdl_destroy( SDL_DATA* sdl );
U8        sdl_beginframe( SDL_DATA* sdl );
U8        sdl_endframe( SDL_DATA* sdl );

void sdl_line( SDL_DATA* sdl, I16 x0, I16 y0, I16 x1, I16 y1, CLR col );
void sdl_rect( SDL_DATA* sdl, I16 x, I16 y, I16 w, I16 h, CLR col );
void sdl_circle( SDL_DATA* sdl, I16 x, I16 y, I16 r, CLR col );
