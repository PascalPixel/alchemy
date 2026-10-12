/* Near miss, differ 12 of 20, registers only: as Func_02003e34 (frame count
 * in r6 and the object in r7 in the native). */
#include "../../../../games/tla/src/field/overlay_653/local.h"

void Func_02003e60(struct ObjectRuntime *object, u32 frames, s32 step)
{
    u32 i;

    for (i = 0; i < frames; i++) {
        object->x += step;
        Battle_WaitMode0(1);
    }
}
