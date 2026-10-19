#ifndef ALCHEMY_STAGED_ACTOR_H
#define ALCHEMY_STAGED_ACTOR_H

/// @file
/// Staged actors: their coordinates, probes and footprints.

#include "types.h"
#include "animspr.h"
#include "static_assert.h"

union StagedActorCoordinate {
  s32 value;
  struct {
    s16 fraction;
    s16 cell;
  } parts;
};

struct StagedActor {
  u8 unk00[6];
  u16 directionAndKind;
  union StagedActorCoordinate x;
  s32 y;
  union StagedActorCoordinate z;
  u8 unk14[0x0e];
  u8 transitionMode;
  u8 unk23;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 moveRateX;
  s32 moveRateZ;
  s32 targetX;
  s32 targetY;
  s32 targetZ;
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

struct StagedActorProbe {
  s32 footprintIndex;
  s32 actorSlot;
  s32 positionX;
  s32 positionY;
  s32 positionZ;
  void (*callback)(void);
};

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

struct StagedActorProbePoint {
  s32 x;
  s32 y;
  s32 z;
};

STATIC_ASSERT(sizeof(struct StagedActorProbePoint) == 0x0c);

#endif
