#ifndef ALCHEMY_OBJECT_RUNTIME_H
#define ALCHEMY_OBJECT_RUNTIME_H

#include "types.h"

struct BattleAnimationState {
    u8 unknown_00[0x28];
    s16 *value_28;
};

struct ObjectRuntime {
    s32 *script;
    s16 step;
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    s32 terrain_height;
    s32 scale_x;
    s32 scale_y;
    u16 radius;
    u8 terrain_id;
    u8 priority_flags;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed_limit;
    s32 acceleration;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    s32 bounce;
    s32 gravity;
    s32 unknown_4c;
    void *animation;
    u8 animation_kind;
    u8 motion_flags;
    u8 unknown_56;
    u8 unknown_57;
    u8 snap_to_target;
    u8 collision_flags;
    u8 action_flags;
    u8 movement_state;
    u8 unknown_5c[0x04];
    void *callback;
    s16 action;
    u16 unknown_66;
    struct ObjectRuntime *linked_object;
    void (*update)(void);
    u8 unknown_70[0x10];
};

typedef char ObjectRuntime_size[
    sizeof(struct ObjectRuntime) == 0x80 ? 1 : -1
];

typedef char ObjectRuntime_collision_flags_offset[
    (u32)&((struct ObjectRuntime *)0)->collision_flags == 0x59 ? 1 : -1
];

void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void ObjectMotion_MoveHalfwayTowardTarget(struct ObjectRuntime *object);
void ObjectMotion_MoveHalfwayTowardTargetCopy(struct ObjectRuntime *object);

struct ObjectRuntime *ObjectTable_Get(u32 object_id);
void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);
void Object_CommitPosition(struct ObjectRuntime *);
void Object_SetMode(struct ObjectRuntime *, s32);
void Object_SetAction(struct ObjectRuntime *, s32);
void Object_RefreshSelector(struct ObjectRuntime *);
void Object_SetActionCallback(struct ObjectRuntime *, s32);
void ObjectMotion_SetActionVariant(u32, s32);
void ObjectMotion_WaitForAnimationChange(u32);
void Object_SetModeById(u32, s32);
void Object_RefreshSelectorById(u32);
void Audio_PlayCue(s32);

extern const u8 Object_LinkedMotionScript[];
extern const u8 Object_OffsetMotionScript[];
extern const u8 Object_VariantMotionScripts[];

#endif
