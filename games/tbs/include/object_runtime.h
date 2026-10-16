#ifndef ALCHEMY_OBJECT_RUNTIME_H
#define ALCHEMY_OBJECT_RUNTIME_H

#include "unit/field/field.h"
#include "object_lookup.h"
#include "types.h"
#include "static_assert.h"

struct ObjectRuntime {
  s32 *script;
  s16 step;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  s32 terrainHeight;
  u8 unk18[0x08];
  u16 radius;
  u8 terrainId;
  u8 unk23;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 speedLimit;
  s32 acceleration;
  s32 targetX;
  s32 targetY;
  s32 targetZ;
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
