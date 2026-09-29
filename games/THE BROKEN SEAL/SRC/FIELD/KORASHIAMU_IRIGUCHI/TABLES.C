#include "STATUS.H"

extern const u32 gKorashiamuIriguchiExits[];

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gKorashiamuIriguchiExits;
}
