#include "CAVE.H"

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gCaveEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return NULL;
}

const u32 *Scene_GetExits(void)
{
    return gCaveExits;
}
