#include "local.h"

void Func_020001c0(void)
{
    Func_020023f8(12);
}

const struct SceneEvent *Scene_GetEvents(void)
{
    return gSceneEvents;
}

s32 Func_020001d4(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 0x1ac) = 0x204;
    Func_02002440(12, 1);
    Func_02002440(13, 1);
    if (GameFlag_Test(0x109) == 0 && gPartyState.entrance == 12)
        Func_02002468(gPartyState.current_owner);
    if (GameFlag_Test(0x109) == 0 && gPartyState.entrance == 13)
        Func_02002468(gPartyState.current_owner);
    return 0;
}

s32 Func_02000250(void)
{
    struct ObjectRuntime *object;
    s32 i;
    s32 x;

    Scene_SetArrivalFlags(0);
    if (GameFlag_Test(0xa04)) {
        object = Object_GetById(14);
        object->motion_flags = 0;
        object->terrain_height = -0x200000;
        object->y = -0x200000;
        Func_020023b0_(14, 0x1480000, 0x2480000);
        Func_02002368(object->x, object->z, 0, 0);
    }
    if (GameFlag_Test(0xa05)) {
        object = Object_GetById(15);
        object->motion_flags = 0;
        object->terrain_height = -0x200000;
        object->y = -0x200000;
        Func_020023b0_(15, 0x2080000, 0x2480000);
        Func_02002368(object->x, object->z, 0, 0);
    }
    Object_GetById(8)->priority_flags |= 32;
    Object_GetById(9)->priority_flags |= 32;
    Object_GetById(10)->priority_flags |= 32;
    Object_GetById(11)->priority_flags |= 32;
    ObjectMotion_SetActionVariant(8, 3);
    ObjectMotion_SetActionVariant(10, 3);
    ObjectMotion_SetActionVariant(9, 3);
    ObjectMotion_SetActionVariant(11, 3);
    Object_GetById(10)->priority_flags |= 2;
    Func_020023d0(8);
    Func_020023d0(9);
    Func_020023d0(10);
    Func_020023d0(11);
    Func_02000c78(8);
    Func_02000c78(9);
    Func_02000c78(10);
    Func_02000c78(11);
    if (GameFlag_Test(0x200)) {
        for (i = 0, x = 0xb00000; i <= 3; i++, x += 0x100000) {
            Lift_(0x1000000, x, 0, 0);
            Func_02002458_(0x1000000, x, 0, 0);
            if (i <= 1)
                Func_02002460_(0x1000000, x, 0, 128);
        }
    }
    return 0;
}

void Func_020003c4(void)
{
    Func_02002400();
}
