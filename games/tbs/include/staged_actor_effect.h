#ifndef ALCHEMY_STAGED_ACTOR_EFFECT_H
#define ALCHEMY_STAGED_ACTOR_EFFECT_H

#include "types.h"

struct StagedActorEffect
{
    u8  unk00[0x0c];
    s32 positionX;
    u8  unk10[4];
    s32 positionZ;
    u8  unk18[0x10];
    s32 elevationRate;
    u8  unk2c[4];
    s32 moveRateX;
    s32 moveRateZ;
    u8  unk38[0x1d];
    u8  motionFlags;
};

struct StagedActorEffectRequest
{
    u8  unk00[2];
    s16 cellX;
    u8  unk04[6];
    s16 cellZ;
};

#define STAGED_EFFECT_OFFSET(type, field) ((u32)&(((type *)0)->field))
typedef char StagedActorEffect_position_x_offset
    [STAGED_EFFECT_OFFSET(struct StagedActorEffect, positionX) == 0x0c ? 1 : -1];
typedef char StagedActorEffect_elevation_rate_offset
    [STAGED_EFFECT_OFFSET(struct StagedActorEffect, elevationRate) == 0x28 ? 1 : -1];
typedef char StagedActorEffect_motion_flags_offset
    [STAGED_EFFECT_OFFSET(struct StagedActorEffect, motionFlags) == 0x55 ? 1 : -1];
typedef char StagedActorEffectRequest_cell_x_offset
    [STAGED_EFFECT_OFFSET(struct StagedActorEffectRequest, cellX) == 0x02 ? 1 : -1];
typedef char StagedActorEffectRequest_cell_z_offset
    [STAGED_EFFECT_OFFSET(struct StagedActorEffectRequest, cellZ) == 0x0a ? 1 : -1];
#undef STAGED_EFFECT_OFFSET

#endif
