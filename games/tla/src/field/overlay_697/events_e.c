#include "ov697.h"

static __inline__ void Map_Stamp(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005820(a, b, c, d, e, f);
}

void Func_02000768(void)
{
    OBJECT_BYTE(Object_GetById(17), 98) = 0;
    GameFlag_SetBit(0x307);
}

void Func_02000784(void)
{
    Func_020059c0();
}

void Func_0200078c(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object != NULL)
        Map_Stamp(14, 27, 1, 1, object->x >> 20, object->z >> 20);
}
