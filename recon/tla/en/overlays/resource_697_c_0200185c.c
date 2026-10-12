/* Draft, differ 2 of 21: the native loads the constant -0x10000 from the pool before it loads y (ldr r2, =0xffff0000; ldr r3, [r5, #12]); this form loads y first. Tried: y -= 0x10000, y += -0x10000, -0x10000 + y, a step temporary behind a one-pass block or an asm barrier, and this pixel inline. */
#include "../../../../games/tla/src/field/overlay_697/ov697.h"

static __inline__ void Object_AddY(struct ObjectRuntime *object, s32 pixels)
{
    object->y += pixels << 16;
}

void Func_0200185c(s32 a, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object->x >> 20 == 8) {
        GameFlag_SetBit(0x9fc);
        object->motion_flags = 0;
        Object_AddY(object, -1);
    }
}
