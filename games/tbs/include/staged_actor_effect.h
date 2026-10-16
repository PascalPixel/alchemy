#ifndef ALCHEMY_STAGED_ACTOR_EFFECT_H
#define ALCHEMY_STAGED_ACTOR_EFFECT_H

#include "types.h"
#include "layout_guard.h"

struct StagedActorEffect {
  u8 unk00[0x0c];
  s32 positionX;
  u8 unk10[4];
  s32 positionZ;
  u8 unk18[0x10];
  s32 elevationRate;
  u8 unk2c[4];
  s32 moveRateX;
  s32 moveRateZ;
  u8 unk38[0x1d];
  u8 motionFlags;
};

struct StagedActorEffectRequest {
  u8 unk00[2];
  s16 cellX;
  u8 unk04[6];
  s16 cellZ;
};

LAYOUT_OFFSET_GUARD(StagedActorEffect_position_x_offset,
                    struct StagedActorEffect, positionX, 0x0c);
LAYOUT_OFFSET_GUARD(StagedActorEffect_elevation_rate_offset,
                    struct StagedActorEffect, elevationRate, 0x28);
LAYOUT_OFFSET_GUARD(StagedActorEffect_motion_flags_offset,
                    struct StagedActorEffect, motionFlags, 0x55);
LAYOUT_OFFSET_GUARD(StagedActorEffectRequest_cell_x_offset,
                    struct StagedActorEffectRequest, cellX, 0x02);
LAYOUT_OFFSET_GUARD(StagedActorEffectRequest_cell_z_offset,
                    struct StagedActorEffectRequest, cellZ, 0x0a);

#endif
