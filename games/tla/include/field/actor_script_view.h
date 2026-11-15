#ifndef ALCHEMY_ACTOR_SCRIPT_VIEW_H
#define ALCHEMY_ACTOR_SCRIPT_VIEW_H

/// @file
/// The actor as its script's wander commands read it.

#include "types.h"
#include "static_assert.h"

/// An actor as its actor script commands see it: the script and its
/// place, position, motion toward a target, and the home it wanders around.
struct ActorScriptView {
  const s32 *script; ///< null when the object is free
  u16 scriptCursor;  ///< in words
  /// The object's facing, 1/65536 turns: the heading the wander commands
  /// wander around, and where SC_FACE_WATCHED stores the angle to the watched
  /// object.
  u16 scriptValue;
  s32 x;                            ///< 16.16 fixed-point pixels
  s32 y;                            ///< 16.16 fixed-point pixels
  s32 z;                            ///< 16.16 fixed-point pixels
  s32 groundHeight;                 ///< 16.16 fixed-point pixels
  u8 unusedAfterGroundHeight[0x0a]; ///< the x and y scales and the radius; not
                                    ///< named here
  u8 layer;            ///< the map layer its ground height is read from
  u8 unusedAfterLayer; ///< Actor.priorityFlags; not named here
  s32 velocityX;       ///< 16.16 fixed-point pixels per frame
  s32 velocityY;       ///< 16.16 fixed-point pixels per frame
  s32 velocityZ;       ///< 16.16 fixed-point pixels per frame
  s32 speedLimit;      ///< 16.16 fixed-point pixels per frame
  s32 acceleration;    ///< 16.16 fixed-point pixels per frame, per frame
  s32 targetX;         ///< 16.16 fixed-point pixels; 0x80000000 when unset
  s32 targetY;         ///< 16.16 fixed-point pixels; 0x80000000 when unset
  s32 targetZ;         ///< 16.16 fixed-point pixels; 0x80000000 when unset
  u8 unusedAfterTargetZ[0x0c]; ///< bounce, gravity and a spare word; not named
                               ///< here
  void *animation; ///< its sprite, or its list of sprites, by animationKind
  u8 spriteStorageKind; ///< 1 one sprite, 2 a group of sprites
  u8 flags;             ///< Actor.motionFlags
  /// The arrival axis, a byte no TLA code names, and the snap flag; not named
  /// here.
  u8 unusedAfterFlags[0x03]; ///< no code names it
  /// Actor.collisionFlags. SC_WANDER_HOME sets bit 1 while it roams
  /// inside its home radius and clears it on heading home.
  u8 flags59;
  u8 steeringFlags;           ///< Actor.steeringFlags: bit 0 faces its motion
  u8 paused;                  ///< Actor.paused
  u8 unusedAfterPaused[0x02]; ///< not named here
  /// The object script's wait, in frames. SC_WANDER sets 1 when it turns
  /// back.
  u16 turnedBack;
  void *callback; ///< not named by TLA code through this struct
  /// Its home, in pixels: objects are put back there. The bytes are
  /// Actor's action and unk66 too.
  ///@{
  s16 homeX;
  s16 homeZ;
  ///@}
  /// The object SC_FACE_WATCHED and SC_MOVE_TO_WATCHED aim at.
  struct ActorScriptView *watchedActor;
  u8 unusedAfterWatchedActor[0x04]; ///< the update hook (Actor.update); not
                                    ///< named here
};

STATIC_ASSERT(sizeof(struct ActorScriptView) == 0x70);
STATIC_ASSERT(offsetof(struct ActorScriptView, script) == 0x00);
STATIC_ASSERT(offsetof(struct ActorScriptView, scriptCursor) == 0x04);
STATIC_ASSERT(offsetof(struct ActorScriptView, scriptValue) == 0x06);
STATIC_ASSERT(offsetof(struct ActorScriptView, x) == 0x08);
STATIC_ASSERT(offsetof(struct ActorScriptView, speedLimit) == 0x30);
STATIC_ASSERT(offsetof(struct ActorScriptView, watchedActor) == 0x68);

#endif
