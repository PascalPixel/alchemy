#include "HEYA.H"

/* The room's tables, which the entry veneers publish to the map engine. */
extern const struct SceneEntrance gToretoHeyaEntrances[];
extern const struct SceneRegion gToretoHeyaRegions[];
extern const u32 gToretoHeyaExits[];
extern const struct ScenePlacement gToretoHeyaPlacements[];

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gToretoHeyaEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return gToretoHeyaRegions;
}

const u32 *Scene_GetExits(void)
{
    return gToretoHeyaExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gToretoHeyaPlacements;
}
