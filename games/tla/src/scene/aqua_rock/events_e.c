#include "local.h"

const u8 *Func_020013b8(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov67bRoom1)
        return Data_02003220;
    if (scene == (s32)&SceneId_Ov67bRoom2)
        return Data_020032d4;
    return Data_02003214;
}
