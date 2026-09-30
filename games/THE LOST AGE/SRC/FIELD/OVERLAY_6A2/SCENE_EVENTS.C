#include "TYPES.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gSceneEvents[];

const struct SceneEvent *Scene_GetEvents(void)
{
    return gSceneEvents;
}
