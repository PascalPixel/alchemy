#ifndef TLA_OBJECT_DISPATCH_H
#define TLA_OBJECT_DISPATCH_H

#include "types.h"
#include "system.h"

struct AnimationObject;
struct AnimationEntry;

struct DispatchChild {
    u8 unk00[5];
    u8 unk05Low : 2;
    u8 stateFlags : 2;
    u8 unk05High : 4;
    u8 unk06[0x0b];
    u8 unk11Low : 1;
    u8 displayFlag : 1;
    u8 unk11High : 6;
    s16 value12;
    u8 unk14[5];
    u8 dirty;
    u8 value1a;
    u8 count;
    u8 unk1c[0x0c];
    struct AnimationEntry *entries[4];
};

struct FieldSprite;

struct DispatchObject {
    u32 value00;
    s16 value04;
    u8 unk06[0x1d];
    u8 priorityFlags;
    u8 unk24[0x0c];
    s32 value30;
    s32 value34;
    u8 unk38[0x18];
    union {
        struct DispatchChild *child;
        struct DispatchChild **children;
        struct FieldSprite *sprite;
        struct AnimationObject *animation;
    } target;
    u8 kind;
    u8 unk55[2];
    u8 value57;
    u8 unk58[3];
    u8 value5b;
    u8 unk5c;
    u8 value5d;
    u8 unk5e[6];
    s16 value64;
    u8 unk66[2];
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

extern const s32 gObjectDispatch_Table4[];
extern const s32 gObjectDispatch_Table6[];

#endif
