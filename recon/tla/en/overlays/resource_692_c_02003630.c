/* Near miss of Func_02003630 of overlay 692 (31 instructions, 6 differ): the native loads the
 * object's y into r3 and the owner's into r2; the draft the other way round. Direct
 * operands load the owner first (14 differ); the flags pointer must come after the two loads. */
#include "local.h"

void Func_02003630(struct ObjectRuntime *object)
{
    struct ObjectRuntime *owner = Object_GetById(gPartyState.current_owner);
    s32 y = *(s16 *)((u8 *)object + 14);
    s32 owner_y = *(s16 *)((u8 *)owner + 14);
    u8 *flags = &object->priority_flags;

    if (y != owner_y || *(s16 *)((u8 *)owner + 18) < *(s16 *)((u8 *)object + 18)) {
        *flags = 1;
    } else {
        *flags = 0;
        Object_SetSpritePriority(object, 3);
    }
}
