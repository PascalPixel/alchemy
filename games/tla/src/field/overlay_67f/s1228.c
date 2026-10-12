#include "ov67f.h"

void Func_02001228(void)
{
    Func_02001488();
    Func_020014e8(0);
    Func_02001520();
    if (gPartyState.scene == (s32)&SceneId_Ov67FRoom2)
        Func_02000ee8(2, 50, 13, Func_02000eac(2, 50, 13) & 255);
    Func_02001490();
}

void Func_02001270(void)
{
    Func_02001488();
    Call6((void (*)())Func_02001460, 7, 2, 8, 7, 7, 12);
    Call6((void (*)())Func_02001450, 7, 66, 7, 76, 8, 9);
    GameFlag_SetBit(0x941);
    Func_02001490();
}

u8 *Func_020012b0(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov67FRoom1)
        return Data_020018ec;
    return Data_020019ac;
}
