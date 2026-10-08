#ifndef TLA_OBJECT_DISPATCH_H
#define TLA_OBJECT_DISPATCH_H

#include "types.h"
#include "system.h"

struct AnimationObject;
struct AnimationEntry;

struct DispatchChild {
    u8 unknown_00[5];
    u8 unknown_05_low : 2;
    u8 state_flags : 2;
    u8 unknown_05_high : 4;
    u8 unknown_06[0x0b];
    u8 unknown_11_low : 1;
    u8 display_flag : 1;
    u8 unknown_11_high : 6;
    s16 value_12;
    u8 unknown_14[5];
    u8 dirty;
    u8 value_1a;
    u8 count;
    u8 unknown_1c[0x0c];
    struct AnimationEntry *entries[4];
};

struct FieldSprite;

struct DispatchObject {
    u32 value_00;
    s16 value_04;
    u8 unknown_06[0x1d];
    u8 priority_flags;
    u8 unknown_24[0x0c];
    s32 value_30;
    s32 value_34;
    u8 unknown_38[0x18];
    union {
        struct DispatchChild *child;
        struct DispatchChild **children;
        struct FieldSprite *sprite;
        struct AnimationObject *animation;
    } target;
    u8 kind;
    u8 unknown_55[2];
    u8 value_57;
    u8 unknown_58[3];
    u8 value_5b;
    u8 unknown_5c;
    u8 value_5d;
    u8 unknown_5e[6];
    s16 value_64;
    u8 unknown_66[2];
    s32 argument;
    void (*update)(struct DispatchObject *object);
};

s32 Animation_ApplyChildArgument(struct DispatchChild *, s32);
void Animation_ApplyChildValue(struct DispatchChild *, s32);
s32 InitializeAnimationObjects(struct DispatchChild *);
s32 ResourceMetadata_Register(struct DispatchChild *, s32);
void ObjectDispatch_Initialize(struct DispatchObject *, const s32 *);
void ObjectDispatch_SetSingleChildFlagsFar(struct DispatchObject *, u32);
void Animation_ApplyChildValuesToRecord(struct DispatchChild *, u32);
void Animation_ApplyChildValuesToRecordFar(struct DispatchChild *, u32);
void Animation_ApplyChildValues(struct DispatchObject *, u32);
void Animation_ApplyChildValuesFar(struct DispatchObject *, u32);
void ObjectDispatch_SetSpritePriority(struct DispatchObject *, s32);

extern const s32 ObjectDispatch_Table4[];
extern const s32 ObjectDispatch_Table6[];

#endif
