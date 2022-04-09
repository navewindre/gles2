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

  game->shaders.gl2d = (U8*)(game->shaders.gl2d_texcoord) - sizeof( GL_PROGRAM );

  return game;
}

void render_watch_arm( GL_PROGRAM* gl, F32 length, F32 progress, CLR col ) {
  VEC2 offset = m_radial_offset( progress * 360.f - 90.f, length );
  gl_2d_line( gl, (VEC2){ 240, 240 }, (VEC2){ 240 + offset.x, 240 + offset.y }, col );
}

void game_main_loop( GAME_DATA* game ) {
  static F32 zoom = 2.99999;
  static U8  dir = 0;
  while( 1 ) {
    I64 tick = m_tickcount();  
  
    GL_DATA* gl = game->gl;
    GL_PROGRAM* gl2d = game->shaders.gl2d;
    GL_PROGRAM* gl2d_texcoord = game->shaders.gl2d_texcoord;

    gl_beginframe( gl );

    VEC2       offset;
    CLR        col = { 1.f, 1.f, 1.f, 1.f };
    time_t     rawtime;
    struct tm* tmi;

    time( &rawtime );
    tmi = localtime( &rawtime );
    
    if( !dir ) {
      zoom -= 0.001f;
      if( zoom < -4.f )
        dir = 1;
    }
    else {
      zoom += 0.001f;
      if( zoom > 4.f )
        dir = 0;
    };

    gl_2d_fcircle( gl2d_texcoord, (VEC2){ 240, 240 }, 150, (CLR){ zoom, 1.f, 1.f, 1.f } );
    gl_2d_circle( gl2d, (VEC2){ 240, 240 }, 150, (CLR){ 0.5, 0, 0.5, 1.f } );
    render_watch_arm( gl2d, 75 , (F32)( tmi->tm_hour % 12 ) / 12.f, (CLR){ 0.5f, 0, 0.5f, 1.f } );
    render_watch_arm( gl2d, 100, (F32)tmi->tm_min / 60.f, (CLR){ 0, 0, 0.5f, 1.f } );
    render_watch_arm( gl2d, 125, (F32)( tick % 60000 ) / 60000, (CLR){ 1.f, 0, 0, 1.f } );

    gl_endframe( gl );
    
    tick = m_tickcount() - tick;
    printf( "time: %02d:%02d:%02d frametime: %lld ms %f\n", 
      tmi->tm_hour, 
      tmi->tm_min, 
      tmi->tm_sec, 
      tick,
      zoom
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
