#include "ov674.h"

void Func_020030b4(void)
{
    struct ObjectRuntime *spawned;

    if (gPartyState.entrance == 3)
        GameFlag_ClearBit(0x12f);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(10), 0);
    Object_SetModeById(10, 4);
    if (GameFlag_Test(0xaa3)) {
        spawned = (struct ObjectRuntime *)Value4((s32 (*)())Func_02003df4, 489, 0x680000, 0x180000, 0xa00000);
        if (spawned != 0) {
            spawned->motion_flags = 0;
            Func_02003e3c(spawned, 453);
            ObjectDispatch_SetSingleChildFlags(spawned, 0);
            spawned->unknown_5c[0] = 1;
        }
        Call3((void (*)())Func_02003edc, 10, 0x680000, 0xcc0000);
    }
    ObjectMotion_SetActionVariant(11, 1);
    Object_SetPartAttribute(Object_GetById(11), 15);
    Object_GetById(11)->priority_flags |= 2;
}
