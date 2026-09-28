#include "HASHIRA.H"

extern const u32 gTakaraHashiraExits[];

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gTakaraHashiraExits;
}
