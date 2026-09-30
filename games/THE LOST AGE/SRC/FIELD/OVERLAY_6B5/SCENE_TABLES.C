#include "TYPES.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gSceneEntrances[];

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSceneEntrances;
}

/* This scene declares no regions. */
const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}
