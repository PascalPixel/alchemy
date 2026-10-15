#include "jupiter_room_7.h"

void Func_020009f8(s32 id, s32 x, s32 z, s32 dx, s32 dz)
{
    struct ObjectRuntime *object = Object_GetById(id);

    Func_02002ba4(id, (x << 20) + 0x80000, (z << 20) + 0x80000);
    object->motion_flags = 3;
    object->terrain_height = 0xfff00000;
    object->y = 0xfff00000;
    object->velocity_y = 0x80000;
    Call3((void (*)())ObjectMotion_SetSpeedParameters, id, 0x19999, 0xcccc);
    ObjectMotion_OffsetPositionAndResetMotion(id, dx, dz);
    Func_02002c5c(222);
}
