#include "local.h"

const u8 *Func_02000318(void)
{
    s32       scene = gPartyState.scene;
    const u8 *placements;

    if (scene == (s32)&SceneId_Ov690Room5)
        return gData_02002380;
    placements = 0;
    if (scene == (s32)&SceneId_Ov690Room3)
        placements = gData_020023a0;
    return placements;
}
