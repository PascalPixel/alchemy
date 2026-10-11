/*
 * Draft of Func_02000848 (overlay 6A9, 02008848). differ 11: ⚓️ turns the
 * leader into the address of its priority flags in r0 before the compare;
 * the draft copies it to r1. Without the pointer the address is built in
 * each branch.
 */
#include "../../../../games/tla/src/field/overlay_6a9/ov6a9.h"

void Func_02000848(struct FieldActor *object)
{
    struct FieldActor *leader = Object_GetById(gPartyState.current_owner);

    u8 *flags = &leader->priority_flags;

    if (leader->z > object->z)
        *flags |= 4;
    else
        *flags &= ~6;
}
