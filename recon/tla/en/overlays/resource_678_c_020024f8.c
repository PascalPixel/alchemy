/* Draft, differ 4 of 40: the native builds the priority mask 3 before it loads the sprite pointer
 * and masks the priority before it reads the sprite byte; this form loads first. Tried: masking the
 * parameter in its own statement (15), priority & 3 (7), a u32 parameter, an empty barrier before the
 * store, an inline setter taking the sprite or the object (4 each), one-pass blocks around the flag
 * stores or the priority store (6 and 8), the mask in a register-operand barrier (4, masked twice),
 * three minutes of permute (nothing better). */
#include "../../../../games/tla/src/field/overlay_678/local.h"

void Func_020027f8(struct ObjectRuntime *object, s32 a);

void Func_020024f8(struct ObjectRuntime *object, s32 priority)
{
    object->motion_flags = 0;
    object->collision_flags = 0;
    object->priority_flags = 2;
    ((struct FieldSprite *)object->animation)->priority = priority;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_020027f8(object, 2);
    Func_02002800(object, Data_02002af4);
    Object_SetPartAttribute(object, 10);
    object->action = 0;
    object->speed_limit = 0;
    object->acceleration = 0;
}
