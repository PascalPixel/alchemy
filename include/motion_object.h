#ifndef ALCHEMY_MOTION_OBJECT_H
#define ALCHEMY_MOTION_OBJECT_H

#include "types.h"

/*
 * Shared 0x70-byte object used by the motion engine around Func_0800d14c.
 * Only fields whose roles are established by multiple callers are named.
 */
struct MotionObject {
    u8 unknown_00[0x08];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x10];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed_limit;
    s32 acceleration;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    s32 vertical_motion_phase;
    s32 vertical_motion_strength;
    u8 unknown_4c[0x09];
    u8 motion_flags;
    u8 unknown_56[0x02];
    u8 snap_to_target;
    u8 unknown_59;
    u8 auto_face_motion;
    u8 unknown_5b[0x15];
};

struct BattleObjectSlot {
    struct MotionObject *object;
    u8 unknown_04[0x08];
    s32 anchor_x;
    s32 anchor_z;
    u8 unknown_14[0x18];
};

#define MOTION_OBJECT_OFFSET(type, field) \
    ((u32)&(((type *)0)->field))

typedef char MotionObject_size[
    sizeof(struct MotionObject) == 0x70 ? 1 : -1
];
typedef char MotionObject_x_offset[
    MOTION_OBJECT_OFFSET(struct MotionObject, x) == 0x08 ? 1 : -1
];
typedef char MotionObject_velocity_x_offset[
    MOTION_OBJECT_OFFSET(struct MotionObject, velocity_x) == 0x24 ? 1 : -1
];
typedef char MotionObject_target_x_offset[
    MOTION_OBJECT_OFFSET(struct MotionObject, target_x) == 0x38 ? 1 : -1
];
typedef char MotionObject_motion_flags_offset[
    MOTION_OBJECT_OFFSET(struct MotionObject, motion_flags) == 0x55 ? 1 : -1
];
typedef char BattleObjectSlot_size[
    sizeof(struct BattleObjectSlot) == 0x2c ? 1 : -1
];
typedef char BattleObjectSlot_anchor_x_offset[
    MOTION_OBJECT_OFFSET(struct BattleObjectSlot, anchor_x) == 0x0c ? 1 : -1
];

#undef MOTION_OBJECT_OFFSET

#endif
