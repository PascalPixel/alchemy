#include "object_runtime.h"
#include "objdisp.h"

void Object_SetTargetAndCallback(u32 object_id, s32 target_id, const void *callback)
{
    struct ObjectRuntime *first = ObjectTable_Get(object_id);
    struct ObjectRuntime *second = ObjectTable_Get(target_id & 0xff);

    if (first != NULL && second != NULL) {
        first->linked_object = second;
        if (!(target_id & 0x10000)) {
            first->action = 40;
            first->acceleration = second->acceleration * 2;
            first->speed_limit = second->speed_limit;
            first->unknown_57[2] = 0;
        }
        ObjectDispatch_InitializeFar((struct DispatchObject *)first, (u32)callback);
    }
}
