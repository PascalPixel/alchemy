#include "OBJECT_DISPATCH.H"

void Animation_SetStateFlags(struct DispatchObject *object, u32 value)
{
    if (object != NULL && object->kind == 1)
        object->target.child->state_flags = value;
}
