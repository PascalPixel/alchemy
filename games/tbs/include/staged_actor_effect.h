#ifndef ALCHEMY_STAGED_ACTOR_EFFECT_H
#define ALCHEMY_STAGED_ACTOR_EFFECT_H

/// @file
/// A staged actor's effect and its request.

#include "types.h"
#include "static_assert.h"

/// The party leader's actor as Kolima Forest's hop sees it: the coordinates
/// it nudges, its jump speeds and its motion flags.
struct StagedActorEffect {
  u8 unk00[0x0c];
  s32 positionX; ///< 16.16 fixed-point pixels
  u8 unk10[4];
  s32 positionZ; ///< 16.16 fixed-point pixels
  u8 unk18[0x10];
  s32 elevationRate; ///< 16.16 fixed-point pixels per frame
  u8 unk2c[4];
  s32 moveRateX; ///< 16.16 fixed-point pixels per frame
  s32 moveRateZ;
  u8 unk38[0x1d];
  u8 motionFlags;
};

/// The hop's destination, a 16.16 position read as its whole pixels.
struct StagedActorEffectRequest {
  u8 unk00[2];
  s16 cellX; ///< pixels
  u8 unk04[6];
  s16 cellZ; ///< pixels
};

STATIC_ASSERT(offsetof(struct StagedActorEffect, positionX) == 0x0c);
STATIC_ASSERT(offsetof(struct StagedActorEffect, elevationRate) == 0x28);
STATIC_ASSERT(offsetof(struct StagedActorEffect, motionFlags) == 0x55);
STATIC_ASSERT(offsetof(struct StagedActorEffectRequest, cellX) == 0x02);
STATIC_ASSERT(offsetof(struct StagedActorEffectRequest, cellZ) == 0x0a);

#endif
