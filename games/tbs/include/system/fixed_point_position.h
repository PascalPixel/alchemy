#ifndef ALCHEMY_FIXED_POINT_POSITION_H
#define ALCHEMY_FIXED_POINT_POSITION_H

/// @file
/// FixedPointPosition, a position in 16.16 fixed point.

#include "types.h"
#include "static_assert.h"

/// A position in three coordinates, each in 16.16 fixed point.
struct FixedPointPosition {
  s32 x; ///< 16.16 fixed point
  s32 y; ///< 16.16 fixed point
  s32 z; ///< 16.16 fixed point
};

STATIC_ASSERT(sizeof(struct FixedPointPosition) == 0x0c);
STATIC_ASSERT(offsetof(struct FixedPointPosition, x) == 0x00);
STATIC_ASSERT(offsetof(struct FixedPointPosition, y) == 0x04);
STATIC_ASSERT(offsetof(struct FixedPointPosition, z) == 0x08);

#endif
