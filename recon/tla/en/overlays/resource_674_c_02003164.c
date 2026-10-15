#include "../../../../games/tla/src/field/overlay_674/ov674.h"

void Func_02003f4c(s32 actor);
void ObjectMotion_SetActionVariant();

struct Ov674MapWork {
    u8 unknown_000[0x190];
    s32 value_190;
    s32 value_194;
};

void Func_02003164(void)
{
    struct Ov674MapWork *map = (struct Ov674MapWork *)Ram_HeapSlots->map_work;
    struct ObjectRuntime *object = Object_GetById(10);
    struct ObjectRuntime *first;
    struct ObjectRuntime *second;
    struct ObjectRuntime *last;
    s32 set;

    map->value_190 = 0;
    map->value_194 = 0;
    if (gPartyState.entrance == 11)
        GameFlag_ClearBit(0x12f);
    if (!GameFlag_Test(0x109)) {
        object->motion_flags = 0;
        ObjectMotion_SetActionVariant(10, 1);
    }
    Func_02003edc(11, 0, 0);
    Func_02003f4c(12);
    Func_02003f4c(13);
    set = GameFlag_Test(0x212);
    if (set) {
        first = Object_GetById(12);
        second = Object_GetById(13);
        first->motion_flags = 0;
        second->motion_flags = 0;
        ((u8 *)first)[0x62] = 1;
        ((u8 *)second)[0x62] = 1;
        Call6((void (*)())Func_02003e24, 7, 3, 1, 1, 7, 29);
    } else {
        first = Object_GetById(12);
        second = Object_GetById(13);
        first->motion_flags = 0;
        second->motion_flags = 0;
        Func_020020ec(first->x, first->z, 3, 255);
        Func_020020ec(second->x, second->z, 3, 255);
    }
    Func_02003f4c(14);
    last = Object_GetById(14);
    ((u8 *)last)[0x62] = 1;
    last->motion_flags = 0;
    Func_020020ec(last->x, last->z, 1, 255);
    if (GameFlag_Test(0x213))
        Call6((void (*)())Func_02003e24, 5, 2, 1, 2, 5, 28);
}
