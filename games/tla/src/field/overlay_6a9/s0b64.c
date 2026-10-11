#include "ov6a9.h"

s32 Func_02000b64(void)
{
    s32 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov6a9Room2)
        Func_02000d10();
    else if (scene == (s32)&SceneId_Ov6a9Room3)
        Func_02000ea0();
    else if (scene == (s32)&SceneId_Ov6a9Room4)
        Func_02000f50();
    return 0;
}
