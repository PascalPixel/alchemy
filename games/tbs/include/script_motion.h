#ifndef ALCHEMY_SCRIPT_MOTION_H
#define ALCHEMY_SCRIPT_MOTION_H

#include "types.h"
#include "layout_guard.h"

struct ScriptMotionWords {
  s32 word[1];
};

struct ScriptMotionObject {
  const struct ScriptMotionWords *script;
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
  void (*hook)(struct ScriptMotionObject *);
};

LAYOUT_SIZE_GUARD(ScriptMotionObject_size, struct ScriptMotionObject, 0x70);
LAYOUT_OFFSET_GUARD(ScriptMotionObject_vertical_offset,
                    struct ScriptMotionObject, vertical, 0x44);
LAYOUT_OFFSET_GUARD(ScriptMotionObject_arrival_axis_offset,
                    struct ScriptMotionObject, arrivalAxis, 0x56);
LAYOUT_OFFSET_GUARD(ScriptMotionObject_wait_offset, struct ScriptMotionObject,
                    wait, 0x5e);
LAYOUT_OFFSET_GUARD(ScriptMotionObject_frozen_offset, struct ScriptMotionObject,
                    frozen, 0x61);
LAYOUT_OFFSET_GUARD(ScriptMotionObject_hook_offset, struct ScriptMotionObject,
                    hook, 0x6c);

#endif
