#include "ov67f.h"

u8 *Func_020010b0(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov67FRoom1)
        return Data_0200176c;
    if (scene == (s32)&SceneId_Ov67FRoom2)
        return Data_02001814;
    return Data_02001754;
}
