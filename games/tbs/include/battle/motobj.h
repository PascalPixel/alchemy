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

struct MotionObject {
  u8 unk00[0x06];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x04];
  s32 scaleX;
  s32 scaleY;
  u16 height;
  u8 unk22[0x02];
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 speedLimit;
  s32 acceleration;
  s32 targetX;
  s32 targetY;
  s32 targetZ;
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
