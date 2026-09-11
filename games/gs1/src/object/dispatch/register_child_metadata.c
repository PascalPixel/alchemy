#include "scene.h"
#include "abi/object/dispatch/register_child_metadata.h"
#include "object_dispatch.h"

void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *object, s32 value)
{
    if (object != 0 && (object->kind & 0xf) == 1) {
        void *child = object->target.child;
        if (value >= 0)
            Obj_Check((s32)child);
    }
}
