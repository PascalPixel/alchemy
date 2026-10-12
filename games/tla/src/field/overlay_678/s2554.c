#include "local.h"

void Func_020024f8(struct ObjectRuntime *object, s32 priority);

void Func_02002554(s32 *position, s32 base)
{
    struct ObjectRuntime *object = Func_02002808_(0x11d, position[0], position[1], position[2] - 0x20000);
    s32 angle;

    if (object != 0) {
        object->bounce = Math_Cosine(((u32)(Random16Far() * 0x3000) >> 16) + 0x3000);
        object->gravity = (u16)Random16Far();
        object->unknown_4c = (u16)(Random16Far() << 1) + base;
        /* FAKEMATCH: the s32 temporary keeps the zero extension and the pool word 0xffff8000; stored directly, the add narrows to 16 bits (movs, lsls for 0x8000, no extension). */
        angle = (u16)Random16Far() - 0x8000;
        object->action = angle;
        Func_020024f8(object, 3);
        object->update = (void (*)(void))Func_02002400;
        Animation_SetStateFlags(object, 1);
    }
}
