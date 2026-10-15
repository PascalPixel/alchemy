#include "local.h"

struct MapWork17 {
    u8 unk00[0x17];
    u8 bits0 : 2;
    u8 mode : 2;
};

void Func_02003a5c(void)
{
    struct MapWork17 *map = (struct MapWork17 *)Ram_HeapSlots->mapWork;

    Data_02006294 = 0;
    Func_02005388(170);
    map->mode = 1;
    Func_02005338(0);
}

void Func_02003a8c(void)
{
    Func_02005390();
    Func_020053a8(0, 0x20f, 8, 9);
    if (GameFlag_Test(0x849)) {
        Func_02005230(10, 0, 0);
    } else {
        ObjectDispatch_SetSingleChildFlags(Object_GetById(10), 0);
        Object_SetModeById(10, 6);
        ObjectMotion_EnableActionAndSetCallback(10, Data_02005550);
    }
    Func_02005338(0);
}

void Func_02003ae4(s32 id, s32 base)
{
    struct ObjectRuntime *object = Object_GetById(id);

    Object_SetModeById(id, 5);
    object->action = base + ((Random16Far() * 400) >> 16);
    object->unk66 = id;
    ObjectMotion_EnableActionAndSetCallback(id, Data_02005548);
}
