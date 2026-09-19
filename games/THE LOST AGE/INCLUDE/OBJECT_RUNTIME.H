#ifndef ALCHEMY_OBJECT_RUNTIME_H
#define ALCHEMY_OBJECT_RUNTIME_H

#include "TYPES.H"

/* Position and movement values are 16.16 fixed point. */
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
    u8 motion_flags;
    u8 unknown_57[3];
    u8 action_flags;
    u8 movement_state;
    u8 unknown_5c[0x04];
    void *callback;
    s16 action;
    u8 unknown_66[0x02];
    struct ObjectRuntime *linked_object;
    u8 unknown_6c[0x14];
};

typedef char ObjectRuntime_size[
    sizeof(struct ObjectRuntime) == 0x80 ? 1 : -1
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
void Object_SetCallback(struct ObjectRuntime *, const void *);
void Object_RefreshSelector(struct ObjectRuntime *);
void Object_SetActionCallback(struct ObjectRuntime *, s32);
void ObjectMotion_SetActionVariant(u32, s32);
void ObjectMotion_WaitForAnimationChange(u32);
void Object_SetModeById(u32, s32);
void Object_RefreshSelectorById(u32);
void Battle_WaitMode0(s32);
void Audio_PlayCue(s32);

extern const u8 Object_LinkedMotionScript[];
extern const u8 Object_OffsetMotionScript[];
extern const u8 Object_VariantMotionScripts[];

#endif
