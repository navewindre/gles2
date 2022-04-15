#include "gl.h"
#include "allocator.h"
#include "file.h"

GL_DATA* gl_init() {
  if( !!SDL_Init( SDL_INIT_EVERYTHING | SDL_VIDEO_OPENGL_ES2 ) )
    return 0;

  GL_DATA* gl = malloc( sizeof( GL_DATA ) );
  gl->window = SDL_CreateWindow( 
    " ", 
    SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED,
    480,
    480,
    0 
  );

  if( !gl->window )
    return 0;

  SDL_SetHint( SDL_HINT_RENDER_DRIVER, "opengles2" );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 0 );
  SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 0 );
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
  
  U32 renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
  gl->renderer = SDL_CreateRenderer( gl->window, -1, renderer_flags );
  if( !gl->renderer )
    return 0;

  gl->ctx = SDL_GL_CreateContext( gl->window );
  if( !gl->ctx )
    return 0;

  gl->programs = 0;
  gl->program_count = 0;

  return gl;
}

void gl_destroy( GL_DATA *gl ) {
  if( gl->programs && gl->program_count ) {
    for( U32 i = 0; i < gl->program_count; ++i )
      gl_program_destroy( gl, &gl->programs[i] );
  }
  if( gl->ctx )
    SDL_GL_DeleteContext( gl->ctx );
  if( gl->window )
    SDL_DestroyWindow( gl->window );

  free( gl );
}

U8 gl_shader_compile( GL_DATA* gl, GL_SHADER* shader ) {
  static char* log_buf = 0;
  if( !log_buf )
    log_buf = malloc( 8192 );

  I32 res;
  shader->id = glCreateShader( (GLenum)shader->type );
  glShaderSource( shader->id, 1, &shader->code, 0 );
  glCompileShader( shader->id );
  glGetShaderiv( shader->id, GL_COMPILE_STATUS, &res );

  if( !res ) {
    glGetShaderInfoLog( shader->id, 8192, 0, log_buf );
    printf( "gl_shader_compile() : error compiling shader. log: \n%s\n%s", log_buf, shader->code );
  
    glDeleteShader( shader->id );
    return 1;
  }

  shader->compiled = 1;
  return 0;
}

void gl_shader_destroy( GL_DATA* gl, GL_SHADER* shader ) {
  if( shader->code )
    free( shader->code );
}

GL_PROGRAM* gl_program_create( GL_DATA* gl, const char* name ) {
  if( !gl->programs )
    gl->programs = list_alloc( sizeof( GL_PROGRAM ) );
  else {
    I32 count = gl->program_count;
    list_insert_after( gl->programs, count );
  }

  GL_PROGRAM* program = list_at( gl->programs, gl->program_count++ );
  char        shader_string[ 256 ];
  char*       shader_code;

  sprintf( shader_string, "../shaders/%s.vsh", name );
  shader_code = file_read( shader_string );
  if( !shader_code ) {
    printf( "gl_program_create() : could not read shader file %s\n", shader_string );
    return 0;
  }

  program->vsh.type = GL_VERTEX_SHADER;
  program->vsh.code = shader_code;

  sprintf( shader_string, "../shaders/%s.fsh", name );
  shader_code = file_read( shader_string );
  if( !shader_code ) {
    printf( "gl_program_create() : could not read shader file %s\n", shader_string );
    return 0;
  }

  program->fsh.type = GL_FRAGMENT_SHADER;
  program->fsh.code = shader_code;

  program->name = name;
  return program;
}

U8 gl_program_compile( GL_DATA* gl, GL_PROGRAM* program ) {
  static char* log_buf = 0;
  if( !log_buf )
    log_buf = malloc( 8192 );

  program->id = glCreateProgram();
  gl_shader_compile( gl, &program->fsh );
  gl_shader_compile( gl, &program->vsh );

  glAttachShader( program->id, program->fsh.id );
  glAttachShader( program->id, program->vsh.id );
  
  I32 status;
  glLinkProgram( program->id );
  glGetProgramiv( program->id, GL_LINK_STATUS, &status );
  if( !status ) {
    glGetProgramInfoLog( program->id, 8192, 0, log_buf );
    printf( "gl_program_compile() : error compiling program %s. log: \n%s\n", 
      program->name, 
      log_buf 
    );

    return 1;
  }

  glGenBuffers( 1, &program->vbuffer );
  return 0;
}

void gl_program_destroy( GL_DATA* gl, GL_PROGRAM* program ) {
  if( program->fsh.compiled )
    gl_shader_destroy( gl, &program->fsh );
  if( program->vsh.compiled )
    gl_shader_destroy( gl, &program->vsh );

  if( gl->program_count == 1 ) {
    free( gl->programs );
    return;
  }

  LIST_ITEM* programs = gl->programs;
  U32 idx = list_find( programs, program );
  if( idx != -1 )
    list_delete_at( programs, gl->program_count );
}

U8 gl_beginframe( GL_DATA* gl ) {
  SDL_SetRenderTarget( gl->renderer, 0 );
  SDL_SetRenderDrawColor( gl->renderer, 0, 0, 0, 255 );
  SDL_RenderClear( gl->renderer );

  return 0;
}

U8 gl_endframe( GL_DATA* gl ) {
  SDL_Event e;
  while( SDL_PollEvent( &e ) ) {
    switch( e.type ) {
      case SDL_QUIT:
        return 1;
      case SDL_KEYDOWN:
        return e.key.keysym.sym == SDLK_ESCAPE;
    }
  }
  
  return 0;
}

