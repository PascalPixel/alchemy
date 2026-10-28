#ifndef ALCHEMY_ACTOR_ARM_VIEW_H
#define ALCHEMY_ACTOR_ARM_VIEW_H

/// @file
/// The words of an actor's script as the motion loops index them, and
/// ActorArmView, struct Actor as update.c reads it.

#include "types.h"
#include "static_assert.h"

struct Actor;

// UB: scripts have variable lengths, so a one-word member cannot describe
// them. Keep the native view outside UBFIX: a flat word pointer changes one
// Thumb load's base/index order; a pointer to an unbounded array does too.
#ifdef UBFIX
typedef s32 ActorScriptWords;
#else
/// The words of an actor's script, as its script pointer reads them.
struct ActorScriptWords {
  s32 word[1];
};
typedef struct ActorScriptWords ActorScriptWords;
#endif

/// struct Actor, field for field, as Actor_UpdateAll (update.c) reads it.
/// update.c is ARM code built with GCC 2.95.2, which cannot read struct
/// Actor's anonymous unions, so it keeps this copy of the layout with the
/// script as words and the bounce as a union (MATCHING.md).
struct ActorArmView {
  const ActorScriptWords *script; ///< null when its slot is released
  u16 scriptCursor;               ///< in words; read signed
  u16 facing;                     ///< 1/65536 turns
  s32 x;                          ///< 16.16 fixed-point pixels
  s32 y;                          ///< 16.16 fixed-point pixels
  s32 z;                          ///< 16.16 fixed-point pixels
  s32 groundHeight;               ///< 16.16 fixed-point pixels
  /// Actor's scales and radius; not named here.
  ///@{
  s32 scaleX;
  s32 scaleY;
  u16 radius;
  ///@}
  u8 layer;         ///< the map layer whose ground height it follows
  u8 priorityFlags; ///< Actor's; not named here
  s32 velocityX;    ///< 16.16 fixed-point pixels per frame
  s32 velocityY;    ///< 16.16 fixed-point pixels per frame
  s32 velocityZ;    ///< 16.16 fixed-point pixels per frame
  s32 speedLimit;   ///< 16.16 fixed-point pixels per frame
  /// Added toward the target each frame, or taken off the speed when there is
  /// none, in 16.16 fixed-point pixels per frame.
  s32 acceleration;
  s32 targetX; ///< 16.16 fixed-point pixels; 0x80000000 when unset
  s32 targetY; ///< 16.16 fixed-point pixels; 0x80000000 when unset
  s32 targetZ; ///< 16.16 fixed-point pixels; 0x80000000 when unset
  union {
    /// The share of the falling speed kept on landing, 16.16 fixed point.
    s32 bounce;
    /// A floating actor's step along gActorScript_CycleTable, low six bits.
    s32 phase;
  } vertical;
  /// Taken off velocityY each frame above the ground, 16.16 fixed-point
  /// pixels per frame; also scales a floating actor's bob.
  s32 gravity;
  /// Actor's riseBaseY, sprite and sprite kind; not named here.
  ///@{
  s32 riseBaseY;
  void *sprite;
  u8 spriteStorageKind;
  ///@}
  /// bits: 4 no slowdown on slopes | 3 bigger float | 2 floats | 1 falls |
  /// 0 follows the terrain; zero is free flight in 3D
  u8 motionFlags;
  u8 arrivalAxis;     ///< the ARRIVAL_AXIS_* that decides arrival
  u8 conditionResult; ///< the actor script's last flag test; not named here
  u8 snapToTarget;    ///< nonzero lands it exactly on its target
  u8 collisionFlags;  ///< bit 7: a solid actor refuses its move
  u8 steeringFlags;   ///< bit 0: turns toward its velocity, 1/16 turn a frame
  u8 paused; ///< nonzero skips its script and motion, not its update routine
  /// Actor's flag that keeps its sprite's VRAM, and the actor script's
  /// SC_LOOP count; not named here.
  ///@{
  u8 retainSpriteCache;
  u8 repeatCount;
  ///@}
  s16 wait;   ///< frames left before its script runs again
  u8 refused; ///< frames in a row a solid actor refused its move
  u8 frozen;  ///< nonzero skips steering, terrain, gravity and floating
  /// Actor's two bytes scene code uses, and its timer
  /// (the actor script's parameter), footprint side and watched actor; not
  /// named here.
  ///@{
  u8 riseCounter;
  u8 riseEnabled;
  s16 timer;
  s16 userValue;
  struct ActorArmView *watchedActor;
  ///@}
  /// Called each frame before its script.
  void (*update)(struct Actor *actor);
};

STATIC_ASSERT(sizeof(struct ActorArmView) == 0x70);
STATIC_ASSERT(offsetof(struct ActorArmView, script) == 0x00);
STATIC_ASSERT(offsetof(struct ActorArmView, scriptCursor) == 0x04);
STATIC_ASSERT(offsetof(struct ActorArmView, facing) == 0x06);
STATIC_ASSERT(offsetof(struct ActorArmView, vertical) == 0x44);
STATIC_ASSERT(offsetof(struct ActorArmView, arrivalAxis) == 0x56);
STATIC_ASSERT(offsetof(struct ActorArmView, wait) == 0x5e);
STATIC_ASSERT(offsetof(struct ActorArmView, frozen) == 0x61);
STATIC_ASSERT(offsetof(struct ActorArmView, update) == 0x6c);

#endif
