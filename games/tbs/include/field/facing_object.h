#ifndef ALCHEMY_FACING_OBJECT_H
#define ALCHEMY_FACING_OBJECT_H

/// @file
/// FacingObject, the view of an object that turns to face a target.

#include "types.h"
#include "static_assert.h"

/// A field object seen as one that turns toward its facing target: its facing,
/// its ground position and the target.
struct FacingObject {
  u8 unk00[6];
  u16 facing;    ///< 1/65536 turns
  s32 positionX; ///< 16.16 fixed-point pixels
  u8 unk0c[4];
  s32 positionZ; ///< 16.16 fixed-point pixels
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
