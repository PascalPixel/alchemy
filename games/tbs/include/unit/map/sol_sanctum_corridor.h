#ifndef ALCHEMY_UNIT_SCENE_SOL_SANCTUM_CORRIDOR_H
#define ALCHEMY_UNIT_SCENE_SOL_SANCTUM_CORRIDOR_H

#include "types.h"
#include "map_tables.h"

struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
extern struct SceneEntrance gSolSanctumCorridorEntrances[];
extern u32 gSolSanctumCorridorExits[];
extern struct ScenePlacement gSolSanctumCorridorPlacements[];
extern struct SceneEvent gSolSanctumCorridorEvents[];

#endif
