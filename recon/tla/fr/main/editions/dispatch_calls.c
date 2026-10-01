/* Draft: this dispatcher predates the typed metadata argument and the
 * repaired raw callee ownership. Two wrongly named call targets differed
 * when recorded; the owner correction now resolves them. The old omitted
 * argument prototype is retained as the measured source attempt. */
#ifndef TLA_OBJECT_DISPATCH_H
#define TLA_OBJECT_DISPATCH_H

#include "TYPES.H"

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
    u8 unknown_14[6];
    u8 value_1a;
    u8 unknown_1b[0x0d];
    s16 *animation_index;
};

struct DispatchObject {
    u32 value_00;
    s16 value_04;
    u8 unknown_06[0x2a];
    s32 value_30;
    s32 value_34;
    u8 unknown_38[0x18];
    union {
        struct DispatchChild *child;
        struct DispatchChild **children;
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
    u8 unknown_6c[4];
};

s32 Animation_ApplyChildArgument(struct DispatchChild *, s32);
void Animation_ApplyChildValue(struct DispatchChild *, s32);
s32 InitializeAnimationObjects(struct DispatchChild *);
s32 ResourceMetadata_Register(struct DispatchChild *);
void ObjectDispatch_Initialize(struct DispatchObject *, u32);
s32 Animation_ApplyChildValuesToRecord(struct DispatchChild *);
s32 WaitFrames(s32);

extern const s32 ObjectDispatch_Table4[];
extern const s32 ObjectDispatch_Table6[];

#endif


void ObjectDispatch_ApplyArgumentToChildren(struct DispatchObject *object, s32 argument)
{
    struct DispatchChild **items;
    struct DispatchChild *item;
    s32 count;

    if (object != 0) {
        switch (object->kind & 0xf) {
        case 1:
            Animation_ApplyChildArgument(object->target.child, argument);
            break;
        case 2:
            items = object->target.children;
            count = 3;
            do {
                item = *items++;
                if (item != 0)
                    Animation_ApplyChildArgument(item, argument);
                count--;
            } while (count >= 0);
            break;
        }
    }
}

void ObjectDispatch_ApplyValueToChildren(struct DispatchObject *object, s32 value)
{
    s32 count;
    struct DispatchChild *child;
    struct DispatchChild **children;

    if (object != 0) {
        switch (object->kind & 0xf) {
        case 1:
            Animation_ApplyChildValue(object->target.child, value);
            return;
        case 2:
            children = object->target.children;
            count = 3;
            do {
                child = *children++;
                if (child != 0)
                    Animation_ApplyChildValue(child, value);
                count--;
            } while (count >= 0);
            break;
        }
    }
}

void ObjectDispatch_ApplyPairToChildren(struct DispatchObject *object, s32 argument, s32 value)
{
    struct DispatchChild **items;
    struct DispatchChild *item;
    s32 count;

    if (object != 0) {
        switch (object->kind & 0xf) {
        case 1:
            Animation_ApplyChildArgument(object->target.child, argument);
            Animation_ApplyChildValue(object->target.child, value);
            break;
        case 2:
            items = object->target.children;
            for (count = 3; count >= 0; count--) {
                item = *items++;
                if (item != 0) {
                    Animation_ApplyChildArgument(item, argument);
                    Animation_ApplyChildValue(item, value);
                }
            }
            break;
        }
    }
}

void ObjectDispatch_SetChildField12(struct DispatchObject *object, u32 value)
{
    if (object != 0 && (object->kind & 0xf) == 1)
        object->target.child->value_12 = value;
}

void Animation_SetIndexAndInitObjects(struct DispatchObject *object, s32 index)
{
    struct DispatchChild *child;

    if (object != NULL && (object->kind & 0xf) == 1) {
        child = object->target.child;
        if (index >= 0) {
            *child->animation_index = index;
            InitializeAnimationObjects(child);
        }
    }
}

void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *object, s32 value)
{
    if (object != 0 && (object->kind & 0xf) == 1) {
        struct DispatchChild *child = object->target.child;
        if (value >= 0)
            ResourceMetadata_Register(child);
    }
}
