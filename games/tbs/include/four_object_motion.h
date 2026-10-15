#ifndef ALCHEMY_FOUR_OBJECT_MOTION_H
#define ALCHEMY_FOUR_OBJECT_MOTION_H

#include "types.h"
#include "global_cells.h"

struct FourObjectMotionState
{
    u8    unk000[0x144];
    s16   verticalOrigins[4];
    u8    unk14c[0xd8];
    void *objects[4];
    s16   positionsX[4];
    s16   positionsY[4];
    s32   phases[4];
};

#define FOUR_OBJECT_MOTION_OFFSET(field) ((u32)&(((struct FourObjectMotionState *)0)->field))

typedef char FourObjectMotionState_vertical_origins_offset
    [FOUR_OBJECT_MOTION_OFFSET(verticalOrigins) == 0x144 ? 1 : -1];
typedef char
    FourObjectMotionState_objects_offset[FOUR_OBJECT_MOTION_OFFSET(objects) == 0x224 ? 1 : -1];
typedef char FourObjectMotionState_positions_x_offset[FOUR_OBJECT_MOTION_OFFSET(positionsX) == 0x234
                                                          ? 1
                                                          : -1];
typedef char FourObjectMotionState_positions_y_offset[FOUR_OBJECT_MOTION_OFFSET(positionsY) == 0x23c
                                                          ? 1
                                                          : -1];
typedef char
    FourObjectMotionState_phases_offset[FOUR_OBJECT_MOTION_OFFSET(phases) == 0x244 ? 1 : -1];

#undef FOUR_OBJECT_MOTION_OFFSET

#endif
