#include "TYPES.H"
#include "FIELD_SCENE.H"

/* The waterway's regions and exits, between its scene-dependent getters. */

extern const u32 GomaSuiro_Exits[];

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return GomaSuiro_Exits;
}
