#ifndef ALCHEMY_SCRIPT_MOTION_H
#define ALCHEMY_SCRIPT_MOTION_H

#include "types.h"

struct ScriptMotionWords
{
    s32 word[1];
};

struct ScriptMotionObject
{
    const struct ScriptMotionWords *script;
    u16                             scriptCursor;
    u16                             facing;
    s32                             x;
    s32                             y;
    s32                             z;
    s32                             terrainHeight;
    u8                              unk18[0x0a];
    u8                              terrainId;
    u8                              unk23;
    s32                             velocityX;
    s32                             velocityY;
    s32                             velocityZ;
    s32                             speedLimit;
    s32                             acceleration;
    s32                             targetX;
    s32                             targetY;
    s32                             targetZ;
    union
    {
        s32 bounce;
        s32 phase;
    } vertical;
    s32 gravity;
    u8  unk4c[9];
    u8  motionFlags;
    u8  arrivalAxis;
    u8  unk57;
    u8  snapToTarget;
    u8  collisionFlags;
    u8  steeringFlags;
    u8  paused;
    u8  unk5c[2];
    s16 wait;
    u8  refused;
    u8  frozen;
    u8  unk62[10];
    void (*hook)(struct ScriptMotionObject *);
};

#define SCRIPT_MOTION_OFFSET(field) ((u32) & (((struct ScriptMotionObject *)0)->field))
typedef char ScriptMotionObject_size[sizeof(struct ScriptMotionObject) == 0x70 ? 1 : -1];
typedef char ScriptMotionObject_vertical_offset[SCRIPT_MOTION_OFFSET(vertical) == 0x44 ? 1 : -1];
typedef char
    ScriptMotionObject_arrival_axis_offset[SCRIPT_MOTION_OFFSET(arrivalAxis) == 0x56 ? 1 : -1];
typedef char ScriptMotionObject_wait_offset[SCRIPT_MOTION_OFFSET(wait) == 0x5e ? 1 : -1];
typedef char ScriptMotionObject_frozen_offset[SCRIPT_MOTION_OFFSET(frozen) == 0x61 ? 1 : -1];
typedef char ScriptMotionObject_hook_offset[SCRIPT_MOTION_OFFSET(hook) == 0x6c ? 1 : -1];
#undef SCRIPT_MOTION_OFFSET

#endif
