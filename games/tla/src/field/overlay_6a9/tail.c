#include "ov6a9.h"

void Func_02000f50(void)
{
    if ((u16)(gPartyState.entrance - 5) <= 1) {
        Call6((void (*)())Func_020010ac, 28, 0, 1, 1, 28, 5);
        Call6((void (*)())Func_020010ac, 36, 0, 1, 1, 36, 5);
    }
}

void Func_02000f98(void)
{
    Func_02000454(Data_02001220);
    Object_GetById(8)->priority_flags |= 0x20;
    Object_GetById(9)->priority_flags |= 0x20;
    Object_GetById(10)->collision_flags |= 8;
    Object_GetById(11)->collision_flags |= 8;
}

void Func_02000fe4(void)
{
    Func_020010dc();
    Func_0200113c(0);
    Func_02001164(158);
    Battle_WaitMode0(10);
    Call6((void (*)())Func_02001094, 7, 94, 6, 94, 1, 2);
    Battle_WaitMode0(16);
    Func_02001164(123);
    Func_02001134(8);
    Func_020010e4();
}
