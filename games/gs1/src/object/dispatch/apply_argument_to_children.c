#include "scene.h"
#include "abi/object/dispatch/apply_argument_to_children.h"
#include "object_commands.h"
#include "object_dispatch.h"

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
            Obj_Apply(object->target.child, argument);
            break;
        case 2:
            items = object->target.children;
            count = 3;
            do {
                item = *items++;
                if (item != 0) {
                    Obj_Apply(item, argument);
                }
                count--;
            } while (count >= 0);
            break;
        }
    }
}
