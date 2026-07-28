#ifndef ALCHEMY_OBJECT_RUNTIME_H
#define ALCHEMY_OBJECT_RUNTIME_H

#include "object_lookup.h"
#include "types.h"

/*
 * Runtime object used by the event/action helpers around Func_0808ba1c.
 * Coordinates and movement values are 16.16 fixed-point unless noted.
 * Unnamed storage remains opaque until a stable role is established.
 */
struct ObjectRuntime {
    u8 unknown_00[0x08];
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
    struct ObjectRuntime *linked_object;
    u8 unknown_6c[0x04];
};

typedef char ObjectRuntime_size[
    sizeof(struct ObjectRuntime) == 0x70 ? 1 : -1
];

#endif
