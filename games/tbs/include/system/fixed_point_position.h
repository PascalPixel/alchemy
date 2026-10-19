#ifndef ALCHEMY_FIXED_POINT_POSITION_H
#define ALCHEMY_FIXED_POINT_POSITION_H

/// @file
/// FixedPointPosition, a position in 16.16 fixed point.

#include "types.h"
#include "static_assert.h"

struct FixedPointPosition {
  s32 x;
  s32 y;
  s32 z;
};

STATIC_ASSERT(sizeof(struct FixedPointPosition) == 0x0c);
STATIC_ASSERT(offsetof(struct FixedPointPosition, x) == 0x00);
STATIC_ASSERT(offsetof(struct FixedPointPosition, y) == 0x04);
STATIC_ASSERT(offsetof(struct FixedPointPosition, z) == 0x08);

#endif
