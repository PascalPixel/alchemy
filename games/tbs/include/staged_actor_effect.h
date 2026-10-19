#ifndef ALCHEMY_STAGED_ACTOR_EFFECT_H
#define ALCHEMY_STAGED_ACTOR_EFFECT_H

/// @file
/// A staged actor's effect and its request.

#include "types.h"
#include "static_assert.h"

/// The party leader's actor as Kolima Forest's hop sees it: the coordinates
/// it nudges, its jump speeds and its motion flags.
struct StagedActorEffect {
  /// FieldActor's script pointer, cursor, facing and x; not named here.
  u8 unk00[0x0c];
  /// FieldActor's y, in 16.16 fixed-point pixels: after landing the hop
  /// lowers it a pixel twice and raises it back.
  s32 positionX;
  u8 unk10[4]; ///< FieldActor's z; not named here
  /// FieldActor's terrainHeight, in 16.16 fixed-point pixels, moved with
  /// positionX.
  s32 positionZ;
  u8 unk18[0x10]; ///< FieldActor's scales to velocityX; not named here
  /// FieldActor's velocityY, in 16.16 fixed-point pixels per frame: 4 pixels
  /// as the hop starts.
  s32 elevationRate;
  u8 unk2c[4]; ///< FieldActor's velocityZ; not named here
  /// FieldActor's speed, in 16.16 fixed-point pixels per frame: 3 pixels for
  /// the hop.
  s32 moveRateX;
  /// FieldActor's acceleration, in 16.16 fixed-point pixels per frame: 2
  /// pixels for the hop.
  s32 moveRateZ;
  u8 unk38[0x1d]; ///< FieldActor's targets to its sprite storage kind
  /// ActorMotionFlag bits: the hop clears bits 7 and 0, zeroes them all when
  /// it lands, then restores them.
  u8 motionFlags;
};

/// The hop's destination, a 16.16 position read as its whole pixels.
struct StagedActorEffectRequest {
  u8 unk00[2]; ///< x's fraction; not named by TBS code
  s16 cellX;   ///< pixels
  u8 unk04[6]; ///< y and z's fraction; read whole as a position
  s16 cellZ;   ///< pixels
};

STATIC_ASSERT(offsetof(struct StagedActorEffect, positionX) == 0x0c);
STATIC_ASSERT(offsetof(struct StagedActorEffect, elevationRate) == 0x28);
STATIC_ASSERT(offsetof(struct StagedActorEffect, motionFlags) == 0x55);
STATIC_ASSERT(offsetof(struct StagedActorEffectRequest, cellX) == 0x02);
STATIC_ASSERT(offsetof(struct StagedActorEffectRequest, cellZ) == 0x0a);

#endif
