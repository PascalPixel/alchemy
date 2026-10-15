#include "local.h"

void Func_020004a8(void)
{
    struct ObjectRuntime *object;

    if (GameFlag_Test(0x842) == 0)
    {
        GameFlag_SetBit(0x230);
        Func_020051c0();
        Func_02005370(0);
        ObjectMotion_SetPositionAndReset(4, 104, 252);
        ObjectMotion_ArmCallback_(4, 0xa000, 0);
        Func_02005378_(14, -16, -16, 0xc000);
        ObjectMotion_CommitCurrentPositionAndActivate(14);
        Battle_WaitMode0(20);
        Func_02005280((s32)&MsgKandoreanTemple11);
        Func_02005298(14, 0);
        Func_020052b0(14, 0x2000);
        Battle_WaitMode0(30);
        Object_SetModeById(14, 3);
        Motion_SetModeAndWaitAnimation(4, 3);
        Battle_WaitMode0(20);
        ObjectMotion_SetSpeedParameters_(14, 0x13333, 0x9999);
        Object_SetModeById(14, 2);
        object = Object_GetById(4);
        if (object)
            ObjectMotion_ResetAndSetPosition(14, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
        ObjectMotion_CommitCurrentPositionAndActivate(14);
        Func_02005230(14, 0, 0);
        Func_020051c8();
    }
}
