#include "TYPES.H"
#include "FIELD_SCENE.H"

/* The sanctum's regions and exits, between its scene-dependent getters. */

extern const u32 HaidiaDou_Exits[];

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return HaidiaDou_Exits;
}
