#include "shaman_village.h"

void Func_02000280(void)
{
    Func_02004a34(18, 3, 13);
}

u8 *Func_02000290(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Shaman5)
        return Data_020052b4;
    if (scene == (s32)&SceneId_Shaman6)
        return Data_02005314;
    return Data_02005284;
}
