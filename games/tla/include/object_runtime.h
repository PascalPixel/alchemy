#ifndef ALCHEMY_OBJECT_RUNTIME_H
#define ALCHEMY_OBJECT_RUNTIME_H

#include "types.h"

struct BattleAnimationState {
    u8 unk00[0x28];
    s16 *value28;
};

struct ObjectRuntime {
    s32 *script;
    s16 step;
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    s32 terrainHeight;
    s32 scaleX;
    s32 scaleY;
    u16 radius;
    u8 terrainId;
    u8 priorityFlags;
    s32 velocityX;
    s32 velocityY;
    s32 velocityZ;
    s32 speedLimit;
    s32 acceleration;
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    s32 bounce;
    s32 gravity;
    s32 unk4c;
    void *animation;
    u8 animationKind;
    u8 motionFlags;
    u8 unk56;
    u8 unk57;
    u8 snapToTarget;
    u8 collisionFlags;
    u8 actionFlags;
    u8 movementState;
    u8 unk5c[0x04];
    void *callback;
    s16 action;
    u16 unk66;
    struct ObjectRuntime *linkedObject;
    void (*update)(void);
    u8 unk70[0x10];
};

typedef char ObjectRuntime_size[
    sizeof(struct ObjectRuntime) == 0x80 ? 1 : -1
];

typedef char ObjectRuntime_collision_flags_offset[
    (u32)&((struct ObjectRuntime *)0)->collisionFlags == 0x59 ? 1 : -1
];

void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void ObjectMotion_MoveHalfwayTowardTarget(struct ObjectRuntime *object);
void ObjectMotion_MoveHalfwayTowardTargetCopy(struct ObjectRuntime *object);

struct ObjectRuntime *ObjectTable_Get(u32 objectId);
void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);
void Object_CommitPosition(struct ObjectRuntime *);
void Object_SetMode(struct ObjectRuntime *, s32);
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
