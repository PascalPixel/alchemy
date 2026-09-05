#include "object_dispatch.h"

s32 Func_0800b8ac(s32 child);

void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *object, s32 value)
{
    if (object != 0 && (object->kind & 0xf) == 1) {
        void *child = object->target.child;
        if (value >= 0)
            Func_0800b8ac((s32)child);
    }
}
