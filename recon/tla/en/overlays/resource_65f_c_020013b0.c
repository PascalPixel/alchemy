/* Draft of overlay 65f Func_020013b0. differ 2 of 43: in the first call the
 * listing loads y (ldr r1, [r5, #12]) right after the x offset is shifted
 * and before it is added; this draft loads y one instruction later, after
 * the add. Not yet tried: a barrier between the two loads. The callee is
 * declared with an int fourth argument here: the listing passes 0xf000
 * unextended, where spawn.c's s16 parameter would pass -0x1000. */
#include "../../../../games/tla/src/field/overlay_65f/ov65f.h"

void Func_02001360(s32 x, s32 y, s32 z, s32 value, s32 timer);

void Func_020013b0(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    Func_02001360(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000, 20);
    Func_02005394(151);
    Battle_WaitMode0(20);
    Func_02001360(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000, 20);
    Func_02005394(151);
    Battle_WaitMode0(20);
}
