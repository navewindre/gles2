#pragma once

#include <stdio.h>
#include "allocator.h"

void* file_read( const char* file ) {
  FILE* f = fopen( file, "rb" );
  if( !f )
    return 0;

  fseek( f, 0, SEEK_END );
  U64 size = ftell( f );
  rewind( f );
  if( !size )
    return 0;

  void* block = malloc( size + 1 );
  fread( block, size, 1, f );
  fclose( f );
  
  ( (U8*)block )[size] = 0;
  return block;
}

char** dir_get_entries( const char* dir ) {
  return 0;
}
