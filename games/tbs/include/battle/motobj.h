#ifndef ALCHEMY_MOTION_OBJECT_H
#define ALCHEMY_MOTION_OBJECT_H

/// @file
/// MotionObject, an effect object that moves and draws one sprite.

#include "types.h"
#include "static_assert.h"
#include "battle_session.h"
#include "edition.h"
#include "farcall.h"

union FieldObject;

/// An effect object that moves and draws one sprite: its position, scale,
/// velocity and target, and its per-frame update.
/// It is one of the object slots Object_CreateFar hands out, the same object
/// ObjectEvent and ScriptMotionObject describe; the notes name their fields at
/// the same offsets.
struct MotionObject {
  /// ScriptMotionObject's script and its cursor; not named through this view.
  ///@{
  const s32 *script;
  u16 scriptCursor;
  ///@}
  u16 angle;        ///< 1/65536 turns
  s32 x;            ///< 16.16 fixed-point pixels
  s32 y;            ///< 16.16 fixed-point pixels
  s32 z;            ///< 16.16 fixed-point pixels
  s32 groundHeight; ///< ObjectEvent's: the ground's y
  s32 scaleX;       ///< 16.16 fixed point, 0x10000 full size
  s32 scaleY;       ///< 16.16 fixed point, 0x10000 full size
  u16 height;       ///< half the sprite's box height; ObjectEvent's radius
  u8 layer;         ///< ObjectEvent's; not named through this view
  u8 priorityFlags; ///< ObjectEvent's; not named through this view
  s32 velocityX;    ///< 16.16 fixed-point pixels per frame
  s32 velocityY;    ///< 16.16 fixed-point pixels per frame
  s32 velocityZ;    ///< 16.16 fixed-point pixels per frame
  s32 speedLimit;   ///< 16.16 fixed-point pixels per frame
  /// Horizontal speed gained each frame towards the target, or lost braking
  /// without one, in 16.16 fixed-point pixels per frame.
  s32 acceleration;
  s32 targetX; ///< 16.16 fixed-point pixels; 0x80000000 for no target
  s32 targetY; ///< 16.16 fixed-point pixels
  s32 targetZ; ///< 16.16 fixed-point pixels
  /// With motionFlags bit 1, the 16.16 share of velocityY kept on a bounce;
  /// with bit 2, the float's phase, stepped each frame (ScriptMotionObject's
  /// vertical).
  s32 verticalMotionPhase;
  /// Gravity: taken from velocityY each frame above the ground with
  /// motionFlags bit 1, in 16.16 fixed-point pixels per frame; with bit 2, it
  /// also scales the float.
  s32 verticalMotionStrength;
  s32 riseBaseY; ///< ObjectEvent's; not named through this view
  void *records; ///< one sprite, or a table of up to four (spritesKind)
  /// bits: 3..0 OBJECT_SPRITES_NONE, OBJECT_SPRITES_ONE or OBJECT_SPRITES_GROUP
  u8 spritesKind;
  /// bits: 4 no terrain slowdown | 3 bigger float | 2 floats | 1 falls |
  /// 0 follows terrain
  u8 motionFlags;
  u8 arrivalAxis;     ///< ObjectEvent's; not named through this view
  u8 conditionResult; ///< ObjectEvent's; not named through this view
  u8 snapToTarget;    ///< nonzero: lands exactly on the target, else brakes in
  u8 collisionFlags;  ///< ObjectEvent's; battle units set 3
  u8 autoFaceMotion;  ///< bit 0 turns the object to face where it moves
  /// ObjectEvent's paused to linkedObject; not named through this view.
  ///@{
  u8 paused;
  u8 retainSpriteCache;
  u8 repeatCount;
  u16 wait;
  u8 refused;
  u8 frozen;
  u8 riseCounter;
  u8 riseEnabled;
  s16 timer;
  u16 work;
  struct MotionObject *linkedObject;
  ///@}
  void (*update)(union FieldObject *fieldObject); ///< run first each frame
};

/// A sprite of an animation record, as battle objects set its palette and
/// drawing priority: the start of an AnimationEntry.
struct SpriteEntry {
  /// AnimationEntry's animationId, timer and kind; not named through this
  /// view.
  ///@{
  s16 animationId;
  u16 timer;
  u8 kind;
  ///@}
  u8 palette; ///< AnimationEntry.colorMode, the SpriteColorMode;
              ///< BattleObject.palette
  /// AnimationEntry.priority: the body and weapon 1, the status icon 3, and
  /// the effect sprite 0, or 9 to hide it while the unit is off the ground.
  u8 priority;
};

STATIC_ASSERT(sizeof(struct MotionObject) == 0x70);
STATIC_ASSERT(offsetof(struct MotionObject, scaleX) == 0x18);
STATIC_ASSERT(offsetof(struct MotionObject, x) == 0x08);
STATIC_ASSERT(offsetof(struct MotionObject, velocityX) == 0x24);
STATIC_ASSERT(offsetof(struct MotionObject, targetX) == 0x38);
STATIC_ASSERT(offsetof(struct MotionObject, motionFlags) == 0x55);
STATIC_ASSERT(offsetof(struct MotionObject, records) == 0x50);
STATIC_ASSERT(offsetof(struct MotionObject, spritesKind) == 0x54);
STATIC_ASSERT(offsetof(struct MotionObject, update) == 0x6c);
STATIC_ASSERT(sizeof(struct SpriteEntry) == 0x08);
STATIC_ASSERT(offsetof(struct SpriteEntry, palette) == 0x05);
STATIC_ASSERT(offsetof(struct SpriteEntry, priority) == 0x06);

#endif
