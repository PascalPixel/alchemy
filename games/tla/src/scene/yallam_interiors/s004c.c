#include "yallam_interiors.h"

u8 *Func_0200004c(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov677Room1)
        return Data_02000ca4;
    if (scene == (s32)&SceneId_Ov677Room2)
        return Data_02000dac;
    return Data_02000c8c;
}
