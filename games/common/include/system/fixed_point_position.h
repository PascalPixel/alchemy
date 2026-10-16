#ifndef ALCHEMY_FIXED_POINT_POSITION_H
#define ALCHEMY_FIXED_POINT_POSITION_H

#include "types.h"
#include "layout_guard.h"

struct FixedPointPosition {
  s32 x;
  s32 y;
  s32 z;
};

LAYOUT_SIZE_GUARD(FixedPointPosition_size, struct FixedPointPosition, 0x0c);
LAYOUT_OFFSET_GUARD(FixedPointPosition_x_offset, struct FixedPointPosition, x,
                    0x00);
LAYOUT_OFFSET_GUARD(FixedPointPosition_y_offset, struct FixedPointPosition, y,
                    0x04);
LAYOUT_OFFSET_GUARD(FixedPointPosition_z_offset, struct FixedPointPosition, z,
                    0x08);

#endif
