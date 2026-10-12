#include "room.h"

void Func_020001f4(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object->x >> 20 == 50) {
        GameFlag_SetBit(0xa38);
        object->motion_flags = 3;
    }
}

void Func_0200021c(void)
{
    struct ObjectRuntime *object = Object_GetById(16);

    Func_02005688();
    if (object->x >> 20 == 50) {
        GameFlag_SetBit(0xa38);
        object->motion_flags = 3;
    }
}

void Func_02000248(s32 unused, s32 id)
{
    if (id == 10)
        Object_GetById(10)->motion_flags = 0;
}

void Func_0200025c(s32 unused, s32 id)
{
    if (id == 16) {
        Func_020001f4(unused, 16);
    } else if (id == 10) {
        Object_GetById(10)->motion_flags = 3;
#if !defined(TLA_EDITION_JA) /* 日本版にはない */
        Func_020002ec();
#endif
    }
}
