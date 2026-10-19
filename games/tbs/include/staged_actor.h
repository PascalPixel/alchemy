#ifndef ALCHEMY_STAGED_ACTOR_H
#define ALCHEMY_STAGED_ACTOR_H

/// @file
/// Staged actors: their coordinates, probes and footprints.

#include "types.h"
#include "animspr.h"
#include "static_assert.h"

/// A pushable thing's coordinate, read whole or as its fraction and its whole
/// pixels.
union StagedActorCoordinate {
  s32 value; ///< 16.16 fixed-point pixels
  struct {
    s16 fraction; ///< 1/65536 pixels
    s16 cell;     ///< pixels
  } parts;
};

/// A thing the party leader pushes or rolls (a block, a log or another large
/// thing), and the leader, as the pushing code sees them.
struct StagedActor {
  u8 unk00[6];
  u16 directionAndKind;
  union StagedActorCoordinate x; ///< 16.16 fixed-point pixels
  s32 y;                         ///< 16.16 fixed-point pixels
  union StagedActorCoordinate z; ///< 16.16 fixed-point pixels
  u8 unk14[0x0e];
  u8 transitionMode;
  u8 unk23;
  s32 velocityX; ///< 16.16 fixed-point pixels per frame
  s32 velocityY; ///< 16.16 fixed-point pixels per frame
  s32 velocityZ; ///< 16.16 fixed-point pixels per frame
  s32 moveRateX; ///< 16.16 fixed-point pixels per frame
  s32 moveRateZ;
  s32 targetX; ///< 16.16 fixed-point pixels
  s32 targetY; ///< 16.16 fixed-point pixels
  s32 targetZ; ///< 16.16 fixed-point pixels
  u8 unk44[0x0c];
  struct AnimationObject *animation;
  u8 unk54[5];
  u8 collisionFlags;
  u8 unk5a[8];
  u8 transitionBusy;
  u8 unk63[3];
  s16 verticalMotionDirection;
  struct FieldActor *linkedObject;
  u32 movementCallback;
};

STATIC_ASSERT(sizeof(struct StagedActor) == 0x70);
STATIC_ASSERT(offsetof(struct StagedActor, x) == 0x08);
STATIC_ASSERT(offsetof(struct StagedActor, transitionMode) == 0x22);
STATIC_ASSERT(offsetof(struct StagedActor, animation) == 0x50);
STATIC_ASSERT(offsetof(struct StagedActor, collisionFlags) == 0x59);
STATIC_ASSERT(offsetof(struct StagedActor, transitionBusy) == 0x62);
STATIC_ASSERT(offsetof(struct StagedActor, verticalMotionDirection) == 0x66);
STATIC_ASSERT(offsetof(struct StagedActor, movementCallback) == 0x6c);

/// Where a large thing ahead of the leader would roll to: its footprint, its
/// actor, that position, and a callback to run once it stops.
struct StagedActorProbe {
  s32 footprintIndex;
  s32 actorSlot;
  s32 positionX; ///< 16.16 fixed-point pixels
  s32 positionY; ///< 16.16 fixed-point pixels
  s32 positionZ; ///< 16.16 fixed-point pixels
  void (*callback)(void);
};

/// A rolled thing's position, held in 16.16 fixed-point pixels or in 16-pixel
/// cells.
struct StagedActorProbePosition {
  s32 x;
  s32 y;
  s32 z;
};

struct StagedActorFootprint {
  s32 x0;
  s32 z0;
  s32 x1;
  s32 z1;
};

STATIC_ASSERT(sizeof(struct StagedActorProbe) == 0x18);
STATIC_ASSERT(offsetof(struct StagedActorProbe, positionX) == 0x08);
STATIC_ASSERT(offsetof(struct StagedActorProbe, positionZ) == 0x10);

STATIC_ASSERT(sizeof(struct StagedActorFootprint) == 0x10);

/// The point the leader's pushing callback checks ahead of a large thing.
struct StagedActorProbePoint {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
};

STATIC_ASSERT(sizeof(struct StagedActorProbePoint) == 0x0c);

#endif
