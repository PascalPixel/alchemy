#ifndef ALCHEMY_STAGED_ACTOR_EFFECT_H
#define ALCHEMY_STAGED_ACTOR_EFFECT_H

#include "types.h"

struct StagedActorEffect {
    u8 unknown_00[0x0c];
    s32 position_x;
    u8 unknown_10[4];
    s32 position_z;
    u8 unknown_18[0x10];
    s32 elevation_rate;
    u8 unknown_2c[4];
    s32 move_rate_x;
    s32 move_rate_z;
    u8 unknown_38[0x1d];
    u8 motion_flags;
};

struct StagedActorEffectRequest {
    u8 unknown_00[2];
    s16 cell_x;
    u8 unknown_04[6];
    s16 cell_z;
};

#define STAGED_EFFECT_OFFSET(type, field) ((u32)&(((type *)0)->field))
typedef char StagedActorEffect_position_x_offset[
    STAGED_EFFECT_OFFSET(struct StagedActorEffect, position_x) == 0x0c ? 1 : -1
];
typedef char StagedActorEffect_elevation_rate_offset[
    STAGED_EFFECT_OFFSET(struct StagedActorEffect, elevation_rate) == 0x28 ? 1 : -1
];
typedef char StagedActorEffect_motion_flags_offset[
    STAGED_EFFECT_OFFSET(struct StagedActorEffect, motion_flags) == 0x55 ? 1 : -1
];
typedef char StagedActorEffectRequest_cell_x_offset[
    STAGED_EFFECT_OFFSET(struct StagedActorEffectRequest, cell_x) == 0x02 ? 1 : -1
];
typedef char StagedActorEffectRequest_cell_z_offset[
    STAGED_EFFECT_OFFSET(struct StagedActorEffectRequest, cell_z) == 0x0a ? 1 : -1
];
#undef STAGED_EFFECT_OFFSET

#endif
