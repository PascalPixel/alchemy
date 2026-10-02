#ifndef ALCHEMY_SCRIPT_MOTION_H
#define ALCHEMY_SCRIPT_MOTION_H

#include "TYPES.H"

struct ScriptMotionWords {
    s32 word[1];
};

/* The script slots updated by the field's motion pass. The vertical word is
   a rebound factor for falling objects, or a phase for cyclic motion. */
struct ScriptMotionObject {
    const struct ScriptMotionWords *script;
    u16 script_cursor;
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    s32 terrain_height;
    u8 unknown_18[0x0a];
    u8 terrain_id;
    u8 unknown_23;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed_limit;
    s32 acceleration;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    union {
        s32 bounce;
        s32 phase;
    } vertical;
    s32 gravity;
    u8 unknown_4c[9];
    u8 motion_flags;
    u8 arrival_axis;
    u8 unknown_57;
    u8 snap_to_target;
    u8 collision_flags;
    u8 steering_flags;
    u8 paused;
    u8 unknown_5c[2];
    s16 wait;
    u8 refused;
    u8 frozen;
    u8 unknown_62[10];
    void (*hook)(struct ScriptMotionObject *);
};

#define SCRIPT_MOTION_OFFSET(field) \
    ((u32)&(((struct ScriptMotionObject *)0)->field))
typedef char ScriptMotionObject_size[
    sizeof(struct ScriptMotionObject) == 0x70 ? 1 : -1
];
typedef char ScriptMotionObject_vertical_offset[
    SCRIPT_MOTION_OFFSET(vertical) == 0x44 ? 1 : -1
];
typedef char ScriptMotionObject_arrival_axis_offset[
    SCRIPT_MOTION_OFFSET(arrival_axis) == 0x56 ? 1 : -1
];
typedef char ScriptMotionObject_wait_offset[
    SCRIPT_MOTION_OFFSET(wait) == 0x5e ? 1 : -1
];
typedef char ScriptMotionObject_frozen_offset[
    SCRIPT_MOTION_OFFSET(frozen) == 0x61 ? 1 : -1
];
typedef char ScriptMotionObject_hook_offset[
    SCRIPT_MOTION_OFFSET(hook) == 0x6c ? 1 : -1
];
#undef SCRIPT_MOTION_OFFSET

#endif
