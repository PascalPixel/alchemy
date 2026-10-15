#include "izumo_ruins.h"

u8 *Func_02001070(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov67FRoom1)
        return gData_020016d0;
    if (scene == (s32)&SceneId_Ov67FRoom2)
        return gData_020016f0;
    return 0;
}
