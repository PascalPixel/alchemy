#include "caverns.h"

const u8 *Func_02000344(void)
{
    const u8 *result = NULL;

    if (gPartyState.scene == (s32)&SceneId_Ov6a9Room2)
        result = Data_02001264;
    return result;
}
