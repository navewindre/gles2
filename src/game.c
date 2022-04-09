#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

#include <time.h>
#include <unistd.h>

#include "game.h"
#include "math.h"

GAME_DATA* game_init( GL_DATA* gl ) {
  GAME_DATA* game = malloc( sizeof( GAME_DATA ) );
  game->gl = gl;
  
  game->shaders.gl2d = gl_2d_init( gl, (VEC2){ 480.f, 480.f }, "2d" );
  game->shaders.gl2d_texcoord = gl_2d_init( gl, (VEC2){ 480.f, 480.f }, "2d_texcoord" );

  return game;
}

void render_watch_arm( GL_PROGRAM* gl, F32 length, F32 progress, CLR col ) {
  VEC2 offset = m_radial_offset( progress * 360.f - 90.f, length );
  gl_2d_line( gl, (VEC2){ 240, 240 }, (VEC2){ 240 + offset.x, 240 + offset.y }, col );
}

void game_main_loop( GAME_DATA* game ) {
  U8 run;
  while( 1 ) {
    I64 tick = m_tickcount();  
  
    GL_DATA* gl = game->gl;
    GL_PROGRAM* gl2d = game->shaders.gl2d;
    GL_PROGRAM* gl2d_texcoord = game->shaders.gl2d_texcoord;

    gl_beginframe( gl );

    VEC2       offset;
    CLR        col = { 255, 255, 255, 255 };
    time_t     rawtime;
    struct tm* tmi;

    time( &rawtime );
    tmi = localtime( &rawtime );
    
    gl_2d_fcircle( gl2d_texcoord, (VEC2){ 240, 240 }, 150, (CLR){ 255, 255, 255, 255 } );
    gl_2d_circle( gl2d_texcoord, (VEC2){ 240, 240 }, 150, (CLR){ 128, 0, 128, 255 } );
    render_watch_arm( gl2d, 75 , (F32)( tmi->tm_hour % 12 ) / 12.f, (CLR){ 128, 0, 128, 255 } );
    render_watch_arm( gl2d, 100, (F32)tmi->tm_min / 60.f, (CLR){ 0, 0, 255, 255 } );
    render_watch_arm( gl2d, 125, (F32)( tick % 60000 ) / 60000, (CLR){ 255, 0, 0, 255 } );

    gl_endframe( gl );

    tick = m_tickcount() - tick;
    printf( "time: %02d:%02d:%02d frametime: %lld ms\n", 
      tmi->tm_hour, 
      tmi->tm_min, 
      tmi->tm_sec, 
      tick 
    );

#ifdef EMSCRIPTEN
    emscripten_sleep( 1 );
#else
    usleep( 1 );
#endif
  }
}

void game_destroy( GAME_DATA *game ) {
  free( game );
}
