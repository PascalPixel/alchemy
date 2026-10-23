#ifndef ALCHEMY_FACING_OBJECT_H
#define ALCHEMY_FACING_OBJECT_H

/// @file
/// FacingObject, the view of an object that turns to face a target.

#include "types.h"
#include "static_assert.h"

/// An object seen as one that turns toward its facing target: its facing,
/// its ground position and the target.
struct FacingObject {
  /// ObjectEvent's script and its cursor; not named through this view.
  ///@{
  const s32 *script;
  u16 scriptCursor;
  ///@}
  u16 facing;    ///< 1/65536 turns
  s32 positionX; ///< 16.16 fixed-point pixels
  s32 positionY; ///< ObjectEvent's y; not named through this view
  s32 positionZ; ///< 16.16 fixed-point pixels
  /// ObjectEvent's groundHeight to collisionFlags; not named through this
  /// view.
  u8 terrainToCollision[0x46];
  /// The steering flags (SW_STEERING); bit 0, turning toward its velocity, is
  /// cleared while it turns toward its target.
  u8 facingFlags;
  /// ObjectEvent's paused to riseEnabled; not named through this view.
  ///@{
  u8 paused;
  u8 retainSpriteCache;
  u8 repeatCount;
  u16 wait;
  u8 refused;
  u8 frozen;
  u8 riseCounter;
  u8 riseEnabled;
  ///@}
  /// Object's action parameter (work value 0x23); not read through
  /// this view.
  s16 action;
  /// The linked object (work value 0x25), or null.
  struct FacingObject *facingTarget;
};

STATIC_ASSERT(sizeof(struct FacingObject) == 0x6c);
STATIC_ASSERT(offsetof(struct FacingObject, facing) == 0x06);
STATIC_ASSERT(offsetof(struct FacingObject, positionX) == 0x08);
STATIC_ASSERT(offsetof(struct FacingObject, positionZ) == 0x10);
STATIC_ASSERT(offsetof(struct FacingObject, facingFlags) == 0x5a);
STATIC_ASSERT(offsetof(struct FacingObject, action) == 0x64);
STATIC_ASSERT(offsetof(struct FacingObject, facingTarget) == 0x68);

#endif
