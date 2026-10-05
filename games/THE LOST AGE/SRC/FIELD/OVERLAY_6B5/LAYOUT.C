#include "GLOBAL.H"
#include "TYPES.H"
#include "FIELD_SCENE.H"


const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gScenePlacements;
}
