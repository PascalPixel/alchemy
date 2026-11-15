#ifndef ALCHEMY_POINT_H
#define ALCHEMY_POINT_H

/// @file
/// Point, a position in 16.16 fixed point.

#include "static_assert.h"
#include "types.h"

/// A position in three coordinates, each in 16.16 fixed point.
struct Point {
  s32 x; ///< 16.16 fixed point
  s32 y; ///< 16.16 fixed point
  s32 z; ///< 16.16 fixed point
};

STATIC_ASSERT(sizeof(struct Point) == 0x0c);
STATIC_ASSERT(offsetof(struct Point, x) == 0x00);
STATIC_ASSERT(offsetof(struct Point, y) == 0x04);
STATIC_ASSERT(offsetof(struct Point, z) == 0x08);

#endif
