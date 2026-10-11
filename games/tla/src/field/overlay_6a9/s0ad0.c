#include "ov6a9.h"

const u8 *Func_02000ad0(void)
{
    s32 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov6a9Room1)
        return Data_0200149c;
    if (scene == (s32)&SceneId_Ov6a9Room3)
        return Data_020014d8;
    if (scene == (s32)&SceneId_Ov6a9Room4)
        return Data_020014fc;
    return Data_02001430;
}
