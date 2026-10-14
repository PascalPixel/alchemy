/* Draft, differ 2 of 31: the native loads gPartyState's pool address before it copies the
 * object argument to r5; this form copies first. The halfword loads are the high halves of y and
 * z. Tried: Value1 for Object_GetById, the comparisons inline or through s32 locals (differ 8 to
 * 16), one minute of permute (nothing better). */
#include "../../../../games/tla/src/field/overlay_692/local.h"

void Func_02003630(struct ObjectRuntime *object)
{
    struct ObjectRuntime *owner = Object_GetById(gPartyState.current_owner);
    s16 owner_y = owner->y >> 16;
    s16 y = object->y >> 16;
    u8 *flags = &object->priority_flags;

    if (owner_y != y || (s16)(owner->z >> 16) < (s16)(object->z >> 16)) {
        *flags = 1;
    } else {
        *flags = 0;
        Object_SetSpritePriority(object, 3);
    }
}
