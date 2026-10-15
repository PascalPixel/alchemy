/* Draft, differ 4 of 40: the native builds the priority mask 3 before it loads the sprite pointer
 * and masks the priority before it reads the sprite byte; this form loads first. Tried: masking the
 * parameter in its own statement (15), priority & 3 (7), a u32 parameter, an empty barrier before the
 * store, an inline setter taking the sprite or the object (4 each), one-pass blocks around the flag
 * stores or the priority store (6 and 8), the mask in a register-operand barrier (4, masked twice),
 * three minutes of permute (nothing better). Also tried: the byte written by hand in four shapes (7 to
 * 16); the mask 3 in a local ahead of a barrier (15: the order then matches, but the mask moves to r2
 * and -13 is built from the 2 left in r3). sched2 ranks the sprite load (74) above the 3 (73). */
#include "../../../../games/tla/src/scene/taopo_swamp/local.h"

void Func_020027f8(struct ObjectRuntime *object, s32 a);

void Func_020024f8(struct ObjectRuntime *object, s32 priority)
{
    object->motionFlags = 0;
    object->collisionFlags = 0;
    object->priorityFlags = 2;
    ((struct FieldSprite *)object->animation)->priority = priority;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_020027f8(object, 2);
    Func_02002800(object, Data_02002af4);
    Object_SetPartAttribute(object, 10);
    object->action = 0;
    object->speedLimit = 0;
    object->acceleration = 0;
}
