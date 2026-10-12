#include "ov672.h"

u8 *Func_02000070(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov672Room1)
        return Data_020020f8;
    if (scene == (s32)&SceneId_Ov672Room3)
        return Data_020021e8;
    return Data_02002458;
}

u8 *Func_020000b0(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov672Room1)
        return Data_02002470;
    if (scene == (s32)&SceneId_Ov672Room2)
        return Data_020024a0;
    if (scene == (s32)&SceneId_Ov672Room3)
        return Data_020024dc;
    return Data_02002530;
}
