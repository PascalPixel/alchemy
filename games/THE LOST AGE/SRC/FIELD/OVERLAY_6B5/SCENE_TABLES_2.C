#include "TYPES.H"
#include "FIELD_SCENE.H"

extern const u32 gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];

const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gScenePlacements;
}
