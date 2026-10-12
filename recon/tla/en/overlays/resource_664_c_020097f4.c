/* Near miss of Func_020017f4 (24 instructions, 4 differ): the native starts the lift constant
 * (movs r2, #128) before it loads the target's x and shifts it before the store; the draft
 * loads x first and shifts after the y load. A one-pass block after the flag toggle and a
 * local for the constant change nothing. */
#include "../../../../games/tla/src/field/overlay_664/ov664.h"

void Func_020017f4(struct FieldActor *object)
{
    struct FieldActor *target = (struct FieldActor *)WORD_AT(object, 104);

    object->priority_flags ^= 2 << (gFrameCount & 1);
    object->x = target->x;
    object->y = target->y + 0x80000;
    object->z = target->z;
}
