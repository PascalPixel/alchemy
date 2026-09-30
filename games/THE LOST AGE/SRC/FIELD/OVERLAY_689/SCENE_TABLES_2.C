#include "TYPES.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gScenePlacements[];

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gScenePlacements;
}
