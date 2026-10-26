#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_CORRIDOR_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_CORRIDOR_H

#include "types.h"
#include "map_tables.h"

struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
extern struct SceneEntrance gSolSanctumCorridorEntrances[];
extern u32 gSolSanctumCorridorExits[];
extern struct ObjectEventTemplate gSolSanctumCorridorObjectEventTemplates[];
extern struct MapEvent gSolSanctumCorridorEvents[];

#endif
