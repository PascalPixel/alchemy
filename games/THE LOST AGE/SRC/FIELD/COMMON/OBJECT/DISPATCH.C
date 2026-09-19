#include "OBJECT_DISPATCH.H"

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
