#include "local.h"

void Func_0200061c(struct ObjectRuntime *object);

void Func_02000644(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32 x;

    if (id == 9) {
        x = object->x >> 20;
        if (x == 39) {
#if defined(TLA_EDITION_JA)
            object->motionFlags = 3;
            WaitFrames(15);
            object->motionFlags = 0;
            GameFlag_SetBit(0x95c);
            Func_02003d1c(41, 9, 1, 1, x, id);
#else
            Func_02003d7c();
            Func_02003eac(0);
            Func_0200061c(object);
            GameFlag_SetBit(0x95c);
            Func_02003d1c(41, 9, 1, 1, x, id);
            Func_02003d84();
#endif
        }
    }
}
