#ifndef ALCHEMY_OBJECT_RUNTIME_H
#define ALCHEMY_OBJECT_RUNTIME_H

#include "TYPES.H"

struct ObjectRuntime {
    u8 unknown_00[0x08];
    s32 x;
    s32 y;
    s32 z;
    s32 terrain_height;
    u8 unknown_18[0x0a];
    u8 terrain_id;
    u8 unknown_23;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed_limit;
    s32 acceleration;
    s32 target_x;
    s32 target_y;
    s32 target_z;
};

#endif
