#pragma once

#include <math.h>
#include <time.h>
#include "vector.h"


static const F32 PIRAD = 0.01745329251f;
static const F32 RADPI = 57.2957795131f;

static inline F32 m_deg2rad( float deg ) { return deg * PIRAD; }
static inline F32 m_rad2deg( float rad ) { return rad * RADPI; }

static inline VEC2 m_radial_offset( float degrees, float distance ) {
  F32 rad = m_deg2rad( degrees );
  F32 x = cos( rad );
  F32 y = sin( rad );
 
  VEC2 ret = { x * distance, y * distance };
  return ret;
}

static inline I64 m_tickcount() {
  struct timespec n;
  if( clock_gettime( CLOCK_MONOTONIC, &n ) )
    return 0;

  return n.tv_sec * 1000 + n.tv_nsec / 1000000;
}
