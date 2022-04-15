#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "typedef.h"

typedef struct LIST_ITEM {
  struct LIST_ITEM* rlink;
  struct LIST_ITEM* flink;
  void*             it;
  U32               size;
} LIST_ITEM;

static inline LIST_ITEM* list_alloc( U32 isize ) {
  LIST_ITEM* list = (LIST_ITEM*)malloc( sizeof( LIST_ITEM ) );
  memset( list, 0, sizeof( LIST_ITEM ) );
  list->it = malloc( isize );
  list->size = isize;

  return list;
}

static void* list_at( LIST_ITEM* list, U32 idx ) {
  LIST_ITEM* it = list;
  for( U32 i = 0; i < idx; ++i ) {
    if( !it->flink )
      return 0;

    it = it->flink;
  }

  return it->it;
}

static I32 list_find( LIST_ITEM* list, void* what ) {
  LIST_ITEM* it;
  I32         i;
  for( it = list, i = 0; !!it->flink; it = it->flink, ++i ) {
    if( !memcmp( it, what, list->size ) )
      return i;
  }

  return 0;
}

static void list_free( LIST_ITEM* list ) {
  while( !!list ) {
    LIST_ITEM* next = list->flink;
    free( list );
    list = next;
  }
}

static void list_delete_at( LIST_ITEM* list, U32 idx ) {
  LIST_ITEM* it = list;
  for( U32 i = 0; i < idx; ++i ) {
    assert( !!it->flink );
    it = it->flink;
  }

  if( it->rlink )
    it->rlink->flink = it->flink;

  if( it->flink )
    it->flink->rlink = it->rlink;
}

static LIST_ITEM* list_insert_before( LIST_ITEM* list, U32 idx ) {
  LIST_ITEM* new_it;
  LIST_ITEM* it = list;
  for( U32 i = 0; i < idx; ++i ) {
    assert( !!it->flink );
    it = it->flink;
  }

  new_it = list_alloc( it->size );
  if( it->rlink ) {
    it->rlink->flink = new_it;
    new_it->rlink = it->rlink;
  }

  new_it->flink = it;
  it->rlink = new_it;
  return new_it;
}

static LIST_ITEM* list_insert_after( LIST_ITEM* list, U32 idx ) {
  LIST_ITEM* new_it;
  LIST_ITEM* it = list;
  for( U32 i = 0; i < idx; ++i )
    assert( !!it->flink );
    it = it->flink;

  new_it = list_alloc( it->size );
  if( it->flink ) {
    it->flink->rlink = new_it;
    new_it->flink = it;
  }

  new_it->rlink = it;
  it->flink = new_it;
  return new_it;
}

static inline void* array_alloc( U32 size, U32 count ) {
  return malloc( size * count );
}

static void* array_realloc( void** arr, U32 size, U32 oldcount, U32 newcount ) {
  if( oldcount == newcount )
    return *arr;

  void* new_block  = malloc( newcount * size );
  U32 wanted_count = oldcount < newcount? oldcount * size : newcount * size;
  
  memcpy( new_block, *arr, wanted_count );
  free( *arr );

  *arr = new_block;
  return new_block;
}

static void* array_pop_pos( void** arr, U32 size, U32 count, U32 where ) {
  if( where >= count )
    return arr;

  if( where == count - 1 )
    return array_realloc( arr, size, count, count - 1 );

  PTR ptr = (PTR)*arr;
  U32 copy_size = size;

  ptr += size * where;
  copy_size = size * ( count - where - 1 );

  memcpy( (void*)ptr, (void*)( ptr + size ), copy_size );
  return array_realloc( arr, size, count, count - 1 );
}

inline void* array_pop_last( void** arr, U32 size, U32 count ) {
  return array_pop_pos( arr, size, count, count - 1 );
}

static void* array_push_pos( void** arr, U32 size, U32 count, void* what, U32 where ) {
  if( where > count )
    return 0;

  array_realloc( arr, size, count, count + 1 );
  PTR ptr = (PTR)*arr;
  ptr += where * size;
  
  if( where == count ) {
    memcpy( (void*)ptr, what, size );
    return *arr;
  }
  
  U32   copy_size = size * ( count - where );
  void* tmp_block = malloc( copy_size );

  memcpy( tmp_block, (void*)ptr, copy_size );
  memcpy( (void*)ptr, what, size );
  memcpy( (void*)( ptr + size ), tmp_block, copy_size );
  
  free( tmp_block );
  return *arr;
}

static inline void* array_push_last( void** arr, U32 size, U32 count, void* what ) {
  return array_push_pos( arr, size, count, what, count );
}

static I32 array_find( void** arr, U32 size, U32 count, void* what ) {
  for( U32 i = 0; i < count; ++i ) {
    PTR item_ptr = (PTR)(*arr) + i * size;

    if( !memcmp( (void*)item_ptr, what, size ) )
      return i;
  }

  return -1;
}

#ifndef ARRAY_DEFINES
#define array_pop1( _1 )
#define array_pop2( _1, _2 )
#define array_pop3( arr, size, count ) array_pop_last( arr, size, count )
#define array_pop4( arr, size, count, where ) array_pop_pos( arr, size, count, where )
#define array_pop5( _1, _2, _3, _4, _5 )

#define array_push1( _1 )
#define array_push2( _1, _2 )
#define array_push3( _1, _2, _3 )
#define array_push4( arr, size, count, what ) array_pop_last( arr, size, count, what )
#define array_push5( arr, size, count, where ) array_push_pos( arr, size, count, what, where )

#define array_select( _1, _2, _3, _4, _5, name, ... )
#define array_pop( ... ) array_select( __VA_ARGS__, array_pop1, array_pop2, array_pop3, array_pop4, array_pop5 )

#define array_push( ... ) array_select( __VA_ARGS__, array_push1, array_push2, array_push3, array_push4, array_push5 )
#define ARRAY_DEFINES
#endif
