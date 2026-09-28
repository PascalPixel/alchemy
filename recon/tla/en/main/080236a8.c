/*
 * Draft: Animation_SetDisplayFlag does not yet match; 4 bytes differ from +0x14.
 * Links as recon/tla/raw/080236a8.s.
 */
#include "OBJECT_DISPATCH.H"

void Animation_SetDisplayFlag(struct DispatchObject *object, u32 value)
{
    if (object != NULL && object->kind == 1)
        object->target.child->display_flag = value;
}
