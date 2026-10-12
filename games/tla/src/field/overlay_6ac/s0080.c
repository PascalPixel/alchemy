#include "ov6ac.h"

const u8 *Func_02000080(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov6ACRoom6)
        return Data_02003508;
    if (scene == (s32)&SceneId_Ov6ACRoom1)
        return Data_02003598;
    return Data_020034f0;
}
