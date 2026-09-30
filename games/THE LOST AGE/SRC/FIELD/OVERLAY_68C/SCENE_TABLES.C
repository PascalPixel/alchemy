#include "TYPES.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gSceneEntrances[];
extern const struct SceneRegion gSceneRegions[];
extern const u32 gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSceneEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return gSceneRegions;
}

const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gScenePlacements;
}
