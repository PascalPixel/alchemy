#ifndef ALCHEMY_FOUR_OBJECT_MOTION_H
#define ALCHEMY_FOUR_OBJECT_MOTION_H

#include "types.h"

/*
 * Shared four-slot projection state used by the Func_080ad274 and
 * Func_080ad508 setup/teardown paths and their frame callbacks.
 */
struct FourObjectMotionState {
    u8 unknown_000[0x144];
    s16 vertical_origins[4];
    u8 unknown_14c[0xd8];
    void *objects[4];
    s16 positions_x[4];
    s16 positions_y[4];
    s32 phases[4];
};

extern struct FourObjectMotionState *Data_03001f2c;

#define FOUR_OBJECT_MOTION_OFFSET(field) \
    ((u32)&(((struct FourObjectMotionState *)0)->field))

typedef char FourObjectMotionState_vertical_origins_offset[
    FOUR_OBJECT_MOTION_OFFSET(vertical_origins) == 0x144 ? 1 : -1
];
typedef char FourObjectMotionState_objects_offset[
    FOUR_OBJECT_MOTION_OFFSET(objects) == 0x224 ? 1 : -1
];
typedef char FourObjectMotionState_positions_x_offset[
    FOUR_OBJECT_MOTION_OFFSET(positions_x) == 0x234 ? 1 : -1
];
typedef char FourObjectMotionState_positions_y_offset[
    FOUR_OBJECT_MOTION_OFFSET(positions_y) == 0x23c ? 1 : -1
];
typedef char FourObjectMotionState_phases_offset[
    FOUR_OBJECT_MOTION_OFFSET(phases) == 0x244 ? 1 : -1
];

#undef FOUR_OBJECT_MOTION_OFFSET

#endif
