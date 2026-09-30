#include "TYPES.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gSceneEntrances[];

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSceneEntrances;
}
