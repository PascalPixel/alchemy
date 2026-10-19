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
/// FieldActor and ScriptMotionObject describe; the notes name their fields at
/// the same offsets.
struct MotionObject {
  u8 unk00[0x06]; ///< ScriptMotionObject's script and scriptCursor
  u16 angle;      ///< 1/65536 turns
  s32 x;          ///< 16.16 fixed-point pixels
  s32 y;          ///< 16.16 fixed-point pixels
  s32 z;          ///< 16.16 fixed-point pixels
  u8 unk14[0x04]; ///< FieldActor's terrainHeight, the ground's y
  s32 scaleX;     ///< 16.16 fixed point, 0x10000 full size
  s32 scaleY;     ///< 16.16 fixed point, 0x10000 full size
  u16 height;     ///< half the sprite's box height; FieldActor's radius
  u8 unk22[0x02]; ///< FieldActor's terrainId and priorityFlags
  s32 velocityX;  ///< 16.16 fixed-point pixels per frame
  s32 velocityY;  ///< 16.16 fixed-point pixels per frame
  s32 velocityZ;  ///< 16.16 fixed-point pixels per frame
  s32 speedLimit; ///< 16.16 fixed-point pixels per frame
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
  u8 unk4c[0x04]; ///< FieldActor's riseBaseY
  void *records;  ///< one sprite, or a table of up to four (recordStorageKind)
  /// bits: 3..0 RECORDS_NONE, RECORDS_SINGLE or RECORDS_TABLE
  u8 recordStorageKind;
  /// bits: 4 no terrain slowdown | 3 bigger float | 2 floats | 1 falls |
  /// 0 follows terrain
  u8 motionFlags;
  u8 unk56[0x02];    ///< ScriptMotionObject's arrivalAxis and unk57
  u8 snapToTarget;   ///< nonzero: lands exactly on the target, else brakes in
  u8 unk59;          ///< FieldActor's collisionFlags; battle units set 3
  u8 autoFaceMotion; ///< bit 0 turns the object to face where it moves
  /// FieldActor's paused, timer, linkedObject and the bytes between; not
  /// named by TBS code through this view.
  u8 unk5b[0x11];
  void (*update)(union FieldObject *object); ///< run first each frame
};

/// A sprite of an animation record, as battle objects set its palette and
/// drawing priority.
struct SpriteEntry {
  u8 unk00[5];
  u8 palette;
  u8 priority;
};

STATIC_ASSERT(sizeof(struct MotionObject) == 0x70);
STATIC_ASSERT(offsetof(struct MotionObject, scaleX) == 0x18);
STATIC_ASSERT(offsetof(struct MotionObject, x) == 0x08);
STATIC_ASSERT(offsetof(struct MotionObject, velocityX) == 0x24);
STATIC_ASSERT(offsetof(struct MotionObject, targetX) == 0x38);
STATIC_ASSERT(offsetof(struct MotionObject, motionFlags) == 0x55);
STATIC_ASSERT(offsetof(struct MotionObject, records) == 0x50);
STATIC_ASSERT(offsetof(struct MotionObject, recordStorageKind) == 0x54);
STATIC_ASSERT(offsetof(struct MotionObject, update) == 0x6c);
STATIC_ASSERT(sizeof(struct SpriteEntry) == 0x08);
STATIC_ASSERT(offsetof(struct SpriteEntry, palette) == 0x05);
STATIC_ASSERT(offsetof(struct SpriteEntry, priority) == 0x06);

#endif
