#include "TYPES.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSceneEntrances;
}

/* This scene declares no regions. */
const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gScenePlacements;
}
