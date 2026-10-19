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
struct MotionObject {
  u8 unk00[0x06];
  u16 angle; ///< 1/65536 turns
  s32 x;     ///< 16.16 fixed-point pixels
  s32 y;     ///< 16.16 fixed-point pixels
  s32 z;     ///< 16.16 fixed-point pixels
  u8 unk14[0x04];
  s32 scaleX; ///< 16.16 fixed point, 0x10000 full size
  s32 scaleY; ///< 16.16 fixed point, 0x10000 full size
  u16 height;
  u8 unk22[0x02];
  s32 velocityX;  ///< 16.16 fixed-point pixels per frame
  s32 velocityY;  ///< 16.16 fixed-point pixels per frame
  s32 velocityZ;  ///< 16.16 fixed-point pixels per frame
  s32 speedLimit; ///< 16.16 fixed-point pixels per frame
  s32 acceleration;
  s32 targetX; ///< 16.16 fixed-point pixels
  s32 targetY; ///< 16.16 fixed-point pixels
  s32 targetZ; ///< 16.16 fixed-point pixels
  s32 verticalMotionPhase;
  s32 verticalMotionStrength;
  u8 unk4c[0x04];
  void *records;
  u8 recordStorageKind;
  u8 motionFlags;
  u8 unk56[0x02];
  u8 snapToTarget;
  u8 unk59;
  u8 autoFaceMotion;
  u8 unk5b[0x11];
  void (*update)(union FieldObject *object);
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
