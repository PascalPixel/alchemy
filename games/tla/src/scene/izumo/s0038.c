#include "izumo.h"
#include "flag_call.h"

void Func_02000038(void)
{
    Func_02000de8(Data_02000e10);
}

void Func_02000048(void)
{
    struct FieldActor *actor = Object_GetById(12);
    s32 x = actor->x >> 20;
    s32 z = actor->z >> 20;

    /* FAKEMATCH: the flag commands go through flag_call.h's inlines, which build each flag id again (direct calls share 0x924 in r5, differ 19), and the block copies through call.h's Call6, as in blocks.c (Func_02000ce8 direct: its stack arguments both pass through r3, differ 4 each). */
    Func_02000df0();
    Func_02000e08(132);
    if (x == 20 && z == 9) {
        if (Flag_IsSet(0x301)) {
            Call6((void (*)())Func_02000ce0, 36, 12, 22, 8, 3, 3);
            Call6((void (*)())Func_02000ce8, 36, 12, 3, 3, 22, 8);
        }
        if (!Flag_IsSet(0x924)) {
            Func_02000d00();
            Func_02000dd0(0);
            Object_AttachWorkTargetToObject(22, 1);
            Func_02000da0();
            Func_02000d88(22, 257, 0);
            Battle_WaitMode0(60);
            ObjectMotion_ArmCallback(22, 0x8000, 0);
            Battle_WaitMode0(60);
            Func_02000d88(22, 256, 0);
            Battle_WaitMode0(60);
            Func_02000d60(MsgTheMisalignedStone);
            Func_02000d70(22, 0);
            Battle_WaitMode0(30);
            Func_02000d88(22, 262, 0);
            Battle_WaitMode0(30);
            Func_02000d70(22, 0);
            ObjectMotion_ArmCallback(22, 0x4000, 0);
            Func_02000d08();
            Flag_Set(0x924);
        }
        Flag_Set(0x250);
    } else if (x == 19 && z == 9) {
        Flag_Clear(0x250);
        if (Flag_IsSet(0x301)) {
            Call6((void (*)())Func_02000ce0, 36, 8, 22, 8, 3, 3);
            Call6((void (*)())Func_02000ce8, 36, 8, 3, 3, 22, 8);
        }
    }
}
