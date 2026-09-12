#include "types.h"
#include "object_dispatch.h"

/* object/dispatch/register_child_metadata.c */
s32 ResourceMetadata_Register(s32 child);

void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *object, s32 value)
{
    if (object != 0 && (object->kind & 0xf) == 1) {
        void *child = object->target.child;
        if (value >= 0)
            ResourceMetadata_Register((s32)child);
    }
}
