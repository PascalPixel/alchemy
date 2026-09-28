/*
 * Draft: Animation_SetStateFlags does not yet match; 4 bytes differ from +0x14.
 * Links as recon/tla/raw/08023680.s.
 */
#include "OBJECT_DISPATCH.H"

void Animation_SetStateFlags(struct DispatchObject *object, u32 value)
{
    if (object != NULL && object->kind == 1)
        object->target.child->state_flags = value;
}
