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
  u8 unk00[6]; ///< FieldActor's script pointer and cursor; not named here
  /// FieldActor's facing, in 1/65536 turns: its top four bits index
  /// gStagedActor_DirectionSteps.
  u16 directionAndKind;
  union StagedActorCoordinate x; ///< 16.16 fixed-point pixels
  s32 y;                         ///< 16.16 fixed-point pixels
  union StagedActorCoordinate z; ///< 16.16 fixed-point pixels
  /// FieldActor's terrainHeight, scales and radius; not named here.
  u8 unk14[0x0e];
  /// FieldActor's terrainId, the map layer its ground height is read from:
  /// the pushing code sets 2 on the thing pushed, and back to 0 after a roll
  /// is probed.
  u8 transitionMode;
  u8 unk23;      ///< FieldActor's priorityFlags; not named here
  s32 velocityX; ///< 16.16 fixed-point pixels per frame
  s32 velocityY; ///< 16.16 fixed-point pixels per frame
  s32 velocityZ; ///< 16.16 fixed-point pixels per frame
  /// FieldActor's speed, in 16.16 fixed-point pixels per frame: 0x3333 for a
  /// pushed block and the leader, 0x8000 for a rolling thing.
  s32 moveRateX;
  /// FieldActor's acceleration, in 16.16 fixed-point pixels per frame: 0x3333
  /// for a pushed block and the leader, 0x1999 for a rolling thing.
  s32 moveRateZ;
  s32 targetX;    ///< 16.16 fixed-point pixels; 0x80000000 when unset
  s32 targetY;    ///< 16.16 fixed-point pixels
  s32 targetZ;    ///< 16.16 fixed-point pixels; 0x80000000 when unset
  u8 unk44[0x0c]; ///< FieldActor's bounce, gravity and riseBaseY; not named
  /// The sprite: its first layer's animation picks the large thing's
  /// footprint from gStagedActor_FootprintKinds.
  struct AnimationObject *animation;
  /// FieldActor's spriteStorageKind, motionFlags and unk56; not named here.
  u8 unk54[5];
  u8 collisionFlags; ///< FieldActor's: bit 0 solid stops a push
  u8 unk5a[8];       ///< FieldActor's steeringFlags to unk61; not named here
  /// FieldActor's riseCounter: a block is not pushed while it is nonzero.
  u8 transitionBusy;
  u8 unk63[3]; ///< FieldActor's riseEnabled and timer; not named here
  s16 verticalMotionDirection;     ///< FieldActor's work; not named by TBS code
  struct FieldActor *linkedObject; ///< FieldActor's; not named through here
  /// FieldActor's update hook: the leader's is StagedActor_StopBlockedMotion
  /// while it pushes a large thing, and 0 after.
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
  /// The large thing's row in gStagedActor_FootprintKinds and
  /// gStagedActor_FootprintBounds, 0 to 5.
  s32 footprintIndex;
  s32 actorSlot; ///< the large thing's actor number, 8 to 65
  s32 positionX; ///< 16.16 fixed-point pixels
  s32 positionY; ///< 16.16 fixed-point pixels
  s32 positionZ; ///< 16.16 fixed-point pixels
  /// Run as the leader follows the rolling thing, when not null; the probe
  /// clears it.
  void (*callback)(void);
};

/// A rolled thing's position, held in 16.16 fixed-point pixels or in 16-pixel
/// cells.
struct StagedActorProbePosition {
  s32 x;
  s32 y; ///< not named by TBS code
  s32 z;
};

/// A footprint's bounds, as gStagedActor_FootprintBounds holds four words a
/// footprint; no TBS code names this struct.
struct StagedActorFootprint {
  s32 x0; ///< left edge, pixels from the thing's position
  s32 z0; ///< top edge, pixels from the thing's position
  s32 x1; ///< right edge, pixels from the thing's position
  s32 z1; ///< bottom edge, pixels from the thing's position
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
