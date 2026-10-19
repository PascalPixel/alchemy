#ifndef ALCHEMY_OBJECT_RUNTIME_H
#define ALCHEMY_OBJECT_RUNTIME_H

/// @file
/// ObjectRuntime, a field object's runtime state, and the object snapshots.

#include "unit/field/field.h"
#include "object_lookup.h"
#include "types.h"
#include "static_assert.h"

/// One of the 64 field objects in the object pool (heap slot 5): its script,
/// position, motion toward a target, animation and update callback.
struct ObjectRuntime {
  s32 *script;
  s16 step;
  u16 angle;         ///< 1/65536 turns
  s32 x;             ///< 16.16 fixed-point pixels
  s32 y;             ///< 16.16 fixed-point pixels
  s32 z;             ///< 16.16 fixed-point pixels
  s32 terrainHeight; ///< 16.16 fixed-point pixels
  u8 unk18[0x08];
  u16 radius; ///< pixels
  u8 terrainId;
  u8 unk23;
  s32 velocityX;  ///< 16.16 fixed-point pixels per frame
  s32 velocityY;  ///< 16.16 fixed-point pixels per frame
  s32 velocityZ;  ///< 16.16 fixed-point pixels per frame
  s32 speedLimit; ///< 16.16 fixed-point pixels per frame
  s32 acceleration;
  s32 targetX; ///< 16.16 fixed-point pixels
  s32 targetY; ///< 16.16 fixed-point pixels
  s32 targetZ; ///< 16.16 fixed-point pixels
  u8 unk44[0x0c];
  void *animation;
  u8 animationKind;
  u8 flags;
  u8 arrivalAxis;
  u8 unk57;
  u8 snapToTarget;
  u8 unk59;
  u8 actionFlags;
  u8 movementState;
  u8 unk5c[0x02];
  u16 unk5e;
  void *callback;
  s16 action;
  s16 unk66;
  struct ObjectRuntime *linkedObject;
  u8 unk6c[0x04];
};

extern struct ObjectRuntime gObjectSnapshots[32];

STATIC_ASSERT(sizeof(struct ObjectRuntime) == 0x70);
STATIC_ASSERT(offsetof(struct ObjectRuntime, arrivalAxis) == 0x56);
STATIC_ASSERT(offsetof(struct ObjectRuntime, snapToTarget) == 0x58);

#endif
