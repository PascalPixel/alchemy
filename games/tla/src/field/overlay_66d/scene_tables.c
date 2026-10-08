#include "local.h"

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSceneEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}

const u8 *Func_0200004c(void)
{
    if (gPartyState.entrance == 10)
        return Data_020008c4;
    if (GameFlag_Test(0x8ff))
        return Data_02000954;
    return Data_020006cc;
}
