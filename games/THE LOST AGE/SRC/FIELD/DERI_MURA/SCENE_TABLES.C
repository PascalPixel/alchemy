#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gDeriMuraEntrances[];

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gDeriMuraEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}
