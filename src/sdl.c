#include "sdl.h"

SDL_DATA* sdl_init() {
  if( !!SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER ) )
    return 0;

  SDL_DATA* sdl = malloc( sizeof( SDL_DATA ) );
  if( !!SDL_CreateWindowAndRenderer( 600, 400, 0, &sdl->window, &sdl->renderer ) ) {
    return 0;
  }

  return sdl;
}

void sdl_destroy( SDL_DATA* sdl ) {
  if( sdl->renderer )
    SDL_DestroyRenderer( sdl->renderer );
  if( sdl->window )
    SDL_DestroyWindow( sdl->window );

  free( sdl );
}

U8 sdl_beginframe( SDL_DATA* sdl ) {
  SDL_SetRenderDrawColor( sdl->renderer, 0, 0, 0, 255 );
  return SDL_RenderClear( sdl->renderer );
}

U8 sdl_endframe( SDL_DATA* sdl ) {
  SDL_RenderPresent( sdl->renderer );
  return 0;
}

void sdl_line( SDL_DATA* sdl, I16 x0, I16 y0, I16 x1, I16 y1, CLR col ) {
  SDL_SetRenderDrawColor( sdl->renderer, col.r, col.g, col.b, col.a );
  SDL_RenderDrawLine( sdl->renderer, x0, y0, x1, y1 );
}
