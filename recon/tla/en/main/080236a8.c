#include "object_dispatch.h"

void Animation_SetDisplayFlag(struct DispatchObject *object, u32 value)
{
    if (object != NULL && object->kind == 1)
        object->target.child->display_flag = value;
}
