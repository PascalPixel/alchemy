#include "scene.h"
#include "abi/object/dispatch/apply_value_to_children.h"
#include "object_dispatch.h"

void ObjectDispatch_ApplyValueToChildren(struct DispatchObject *object, s32 value)
{
    s32 count;
    void *child;
    void **children;

    if (object != 0) {
        switch (object->kind & 0xf) {
        case 1:
            Obj_Apply(object->target.child, value);
            return;
        case 2:
            children = object->target.children;
            count = 3;
            do {
                child = *children++;
                if (child != 0)
                    Obj_Apply(child, value);
                count--;
            } while (count >= 0);
            break;
        }
    }
}
