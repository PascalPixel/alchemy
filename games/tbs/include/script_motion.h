#ifndef ALCHEMY_SCRIPT_MOTION_H
#define ALCHEMY_SCRIPT_MOTION_H

/// @file
/// ScriptMotionObject, an object moved by a field script.

#include "types.h"
#include "static_assert.h"

union FieldObject;

// UB: scripts have variable lengths, so a one-word member cannot describe
// them. Keep the native view outside UBFIX: a flat word pointer changes one
// Thumb load's base/index order; a pointer to an unbounded array does too.
#ifdef UBFIX
typedef s32 ScriptMotionWords;
#else
struct ScriptMotionWords {
  s32 word[1];
};
typedef struct ScriptMotionWords ScriptMotionWords;
#endif

struct ScriptMotionObject {
  const ScriptMotionWords *script;
  u16 scriptCursor;
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 terrainHeight;
  u8 unk18[0x0a];
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
  union {
    s32 bounce;
    s32 phase;
  } vertical;
  s32 gravity;
  u8 unk4c[9];
  u8 motionFlags;
  u8 arrivalAxis;
  u8 unk57;
  u8 snapToTarget;
  u8 collisionFlags;
  u8 steeringFlags;
  u8 paused;
  u8 unk5c[2];
  s16 wait;
  u8 refused;
  u8 frozen;
  u8 unk62[10];
  void (*hook)(union FieldObject *object);
};

STATIC_ASSERT(sizeof(struct ScriptMotionObject) == 0x70);
STATIC_ASSERT(offsetof(struct ScriptMotionObject, script) == 0x00);
STATIC_ASSERT(offsetof(struct ScriptMotionObject, scriptCursor) == 0x04);
STATIC_ASSERT(offsetof(struct ScriptMotionObject, facing) == 0x06);
STATIC_ASSERT(offsetof(struct ScriptMotionObject, vertical) == 0x44);
STATIC_ASSERT(offsetof(struct ScriptMotionObject, arrivalAxis) == 0x56);
STATIC_ASSERT(offsetof(struct ScriptMotionObject, wait) == 0x5e);
STATIC_ASSERT(offsetof(struct ScriptMotionObject, frozen) == 0x61);
STATIC_ASSERT(offsetof(struct ScriptMotionObject, hook) == 0x6c);

#endif
