#ifndef ALCHEMY_OBJECT_RUNTIME_H
#define ALCHEMY_OBJECT_RUNTIME_H

#include "OBJECT_LOOKUP.H"
#include "TYPES.H"

/*
 * イベント・動作処理用オブジェクト。
 * 座標と移動量は特記がなければ16.16固定小数。
 */
struct ObjectRuntime {
    s32 *script;
    s16 step;
    u16 angle;
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
    u8 arrival_axis;
    u8 unknown_57;
    u8 snap_to_target;
    u8 unknown_59;
    u8 action_flags;
    u8 movement_state;
    u8 unknown_5c[0x02];
    u16 unknown_5e;
    void *callback;
    s16 action;
    s16 unknown_66;
    struct ObjectRuntime *linked_object;
    u8 unknown_6c[0x04];
};

typedef char ObjectRuntime_size[
    sizeof(struct ObjectRuntime) == 0x70 ? 1 : -1
];

typedef char ObjectRuntime_arrival_axis_offset[
    (u32)&(((struct ObjectRuntime *)0)->arrival_axis) == 0x56 ? 1 : -1
];
typedef char ObjectRuntime_snap_to_target_offset[
    (u32)&(((struct ObjectRuntime *)0)->snap_to_target) == 0x58 ? 1 : -1
];

void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);

#endif
