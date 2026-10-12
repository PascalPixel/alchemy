#include "ov67f.h"

u8 *Func_02001070(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov67FRoom1)
        return Data_020016d0;
    if (scene == (s32)&SceneId_Ov67FRoom2)
        return Data_020016f0;
    return 0;
}
