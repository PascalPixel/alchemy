#include "scene.h"

void Func_02001288(s32 mode)
{
    struct FieldActor *object;
    s32                height;

    if (mode == 1)
    {
        object = Object_GetById(9);
        Func_02003ff0(35, 39, 13, 11, 3, 3);
        Scene_Call4090(9, 0xe80000, 0xc80000);
        height = Map_GetTerrainHeight(0, object->x, object->z);
        object->y = height;
        object->terrainHeight = height;
        GameFlag_SetBit(0x8ed);
    }
}
