#include "ov697.h"

void Func_02001104(s32 id, s32 a, s32 frame)
{
    if (frame == 30) {
        struct ObjectRuntime *object = Object_GetById(id);
        s32 height;

        object->x = object->action << 16;
        object->z = (s16)object->unknown_66 << 16;
        height = Map_GetTerrainHeight(0, object->x, object->z);
        object->terrain_height = height;
        object->y = height;
        Func_0200078c(id);
        GameFlag_SetBit(id + 0x353);
    }
}

void Func_02001148(s32 a, s32 b)
{
    Func_02001104(16, a, b);
}

void Func_02001158(s32 a, s32 b)
{
    Func_02001104(17, a, b);
}

void Func_02001168(s32 a, s32 b)
{
    Func_02001104(18, a, b);
}
