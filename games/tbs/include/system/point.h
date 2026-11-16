#ifndef ALCHEMY_POINT_H
#define ALCHEMY_POINT_H

/// @file
/// Point, a position in 16.16 fixed point.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

/// A position in three coordinates, each in 16.16 fixed point.
struct Point {
  FIXED x; ///< 16.16 fixed point
  FIXED y; ///< 16.16 fixed point
  FIXED z; ///< 16.16 fixed point
};

STATIC_ASSERT(sizeof(struct Point) == 12);
STATIC_ASSERT(offsetof(struct Point, x) == 0);
STATIC_ASSERT(offsetof(struct Point, y) == 4);
STATIC_ASSERT(offsetof(struct Point, z) == 8);

#endif
