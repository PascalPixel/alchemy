#include "types.h"
#include "scene.h"
#include "object_dispatch.h"
#include "object_commands.h"

/* object/dispatch/find_free_object.c */
extern u8 *gIw;

void *ObjectDispatch_FindFreeObject(void)
{
    u8 *entry = gIw;
    void *ret = 0;
    s32 index = 0;

    while (index <= 63) {
        if (*(u32 *)entry == 0) {
            ret = entry;
            break;
        }
        index++;
        entry += 112;
    }
    return ret;
}

/* object/dispatch/initialize.c */
void ObjectDispatch_Initialize(struct DispatchObject *object, u32 value)
{
    if (object != 0) {
        object->value_04 = 0;
        object->value_00 = value;
        object->value_5b = 0;
        object->value_5d = 0;
        object->value_57 = 0;
    }
}

/* object/dispatch/apply_argument_to_children.c */
void ObjectDispatch_ApplyArgumentToChildren(void *raw_object, s32 argument)
{
    struct DispatchObject *object;
    void **items;
    s32 count;
    void *item;

    object = raw_object;
    if (object != 0) {
        switch (object->kind & 0xf) {
        case 1:
            FunctionHead_0800ba30(object->target.child, argument);
            break;
        case 2:
            items = object->target.children;
            count = 3;
            do {
                item = *items++;
                if (item != 0) {
                    FunctionHead_0800ba30(item, argument);
                }
                count--;
            } while (count >= 0);
            break;
        }
    }
}

/* object/dispatch/apply_value_to_children.c */
void ObjectDispatch_ApplyValueToChildren(struct DispatchObject *object, s32 value)
{
    s32 count;
    void *child;
    void **children;

    if (object != 0) {
        switch (object->kind & 0xf) {
        case 1:
            FunctionHead_0800baf8(object->target.child, value);
            return;
        case 2:
            children = object->target.children;
            count = 3;
            do {
                child = *children++;
                if (child != 0)
                    FunctionHead_0800baf8(child, value);
                count--;
            } while (count >= 0);
            break;
        }
    }
}

/* object/dispatch/apply_pair_to_children.c */
void ObjectDispatch_ApplyPairToChildren(void *arg0, s32 arg1, s32 arg2)
{
    void **items;
    void *item;
    s32 count;

    if (arg0 != 0) {
        switch (*((u8 *)arg0 + 84) & 15) {
        case 1:
            Obj_Run(*(s32 *)((u8 *)arg0 + 80), arg1);
            AnimationObjects_SetField15OnActive(*(s32 *)((u8 *)arg0 + 80), arg2);
            break;
        case 2:
            items = *(void ***)((u8 *)arg0 + 80);
            for (count = 3; count >= 0; count--) {
                item = *items++;
                if (item != 0) {
                    Obj_Run((s32)item, arg1);
                    AnimationObjects_SetField15OnActive((s32)item, arg2);
                }
            }
            break;
        }
    }
}

/* object/dispatch/set_child_field_1e.c */
void ObjectDispatch_SetChildField1e(struct DispatchObject *object, u32 value)
{
    if (object != 0 && (object->kind & 0xf) == 1)
        *(s16 *)((u8 *)object->target.child + 0x1e) = value;
}
