#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/* Daila's tables: where the party appears, and its map regions. */

extern const struct SceneEntrance gDeriMuraEntrances[];

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gDeriMuraEntrances;
}

/* Daila declares no regions. */
const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}
