#ifndef ALCHEMY_FIXED_POINT_POSITION_H
#define ALCHEMY_FIXED_POINT_POSITION_H

#include "types.h"

/* Three 16.16 coordinates consumed by the shared distance helper. */
struct FixedPointPosition {
    s32 x;
    s32 y;
    s32 z;
};

/* The resident integer-square-root routine used by small code overlays. */
typedef s32 (*IwramIntegerSquareRoot)(s32 value);

#define FIXED_POINT_POSITION_OFFSET(field) \
    ((u32)&(((struct FixedPointPosition *)0)->field))
typedef char FixedPointPosition_size[
    sizeof(struct FixedPointPosition) == 0x0c ? 1 : -1
];
typedef char FixedPointPosition_x_offset[
    FIXED_POINT_POSITION_OFFSET(x) == 0x00 ? 1 : -1
];
typedef char FixedPointPosition_y_offset[
    FIXED_POINT_POSITION_OFFSET(y) == 0x04 ? 1 : -1
];
typedef char FixedPointPosition_z_offset[
    FIXED_POINT_POSITION_OFFSET(z) == 0x08 ? 1 : -1
];
#undef FIXED_POINT_POSITION_OFFSET

#endif
