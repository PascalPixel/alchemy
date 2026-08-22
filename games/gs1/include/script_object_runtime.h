#ifndef ALCHEMY_SCRIPT_OBJECT_RUNTIME_H
#define ALCHEMY_SCRIPT_OBJECT_RUNTIME_H

#include "types.h"

/*
 * Script commands attached to runtime objects share one 0x70-byte record.
 * The script cursor occupies the otherwise unnamed first eight bytes of the
 * ordinary runtime-object view.
 */
struct ScriptObjectRuntime {
    const s32 *script;
    u16 script_cursor;
    u16 script_value;
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
    u8 unknown_44[0x0c];
    void *animation;
    u8 animation_kind;
    u8 flags;
    u8 unknown_56[0x04];
    u8 action_flags;
    u8 movement_state;
    u8 unknown_5c[0x04];
    void *callback;
    s16 action;
    u8 unknown_66[0x02];
    struct ScriptObjectRuntime *linked_object;
    u8 unknown_6c[0x04];
};

#define SCRIPT_OBJECT_RUNTIME_OFFSET(field) \
    ((u32)&(((struct ScriptObjectRuntime *)0)->field))

typedef char ScriptObjectRuntime_size[
    sizeof(struct ScriptObjectRuntime) == 0x70 ? 1 : -1
];
typedef char ScriptObjectRuntime_x_offset[
    SCRIPT_OBJECT_RUNTIME_OFFSET(x) == 0x08 ? 1 : -1
];
typedef char ScriptObjectRuntime_speed_limit_offset[
    SCRIPT_OBJECT_RUNTIME_OFFSET(speed_limit) == 0x30 ? 1 : -1
];
typedef char ScriptObjectRuntime_linked_object_offset[
    SCRIPT_OBJECT_RUNTIME_OFFSET(linked_object) == 0x68 ? 1 : -1
];

#undef SCRIPT_OBJECT_RUNTIME_OFFSET

#endif
