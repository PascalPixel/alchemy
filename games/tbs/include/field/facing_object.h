#ifndef ALCHEMY_FACING_OBJECT_H
#define ALCHEMY_FACING_OBJECT_H

/// @file
/// FacingObject, the view of an object that turns to face a target.

#include "types.h"
#include "static_assert.h"

struct FacingObject {
  u8 unk00[6];
  u16 facing;
  s32 positionX;
  u8 unk0c[4];
  s32 positionZ;
  u8 unk14[0x46];
  u8 facingFlags;
  u8 unk5b[9];
  s16 unk64;
  struct FacingObject *facingTarget;
};

STATIC_ASSERT(sizeof(struct FacingObject) == 0x6c);
STATIC_ASSERT(offsetof(struct FacingObject, facing) == 0x06);
STATIC_ASSERT(offsetof(struct FacingObject, positionX) == 0x08);
STATIC_ASSERT(offsetof(struct FacingObject, positionZ) == 0x10);
STATIC_ASSERT(offsetof(struct FacingObject, facingFlags) == 0x5a);
STATIC_ASSERT(offsetof(struct FacingObject, unk64) == 0x64);
STATIC_ASSERT(offsetof(struct FacingObject, facingTarget) == 0x68);

#endif
