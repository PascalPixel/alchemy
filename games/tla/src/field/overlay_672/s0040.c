#include "ov672.h"

u8 *Func_02000040(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov672Room1)
        return Data_0200a08c;
    return 0;
}
