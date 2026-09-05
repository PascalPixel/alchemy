#include "object_dispatch.h"

void ObjectDispatch_SetChildField1e(struct DispatchObject *object, u32 value)
{
    if (object != 0 && (object->kind & 0xf) == 1)
        *(s16 *)((u8 *)object->target.child + 0x1e) = value;
}
