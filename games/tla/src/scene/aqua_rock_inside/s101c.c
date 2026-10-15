#include "local.h"

void Func_0200101c(s32 unused, s32 id)
{
    if ((u32)(id - 13) <= 2)
    {
        struct ObjectRuntime *object = Object_GetById(id);

        Func_02003d1c_(47, 45, 1, 1, object->x >> 20, object->z >> 20);
        GameFlag_SetBit(id + 0x244);
        object->priorityFlags = 0;
    }
}

void Func_02001060(void)
{
    Func_02003d1c_(8, 63, 7, 1, 8, 34);
}
