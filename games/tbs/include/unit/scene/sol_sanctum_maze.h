#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_MAZE_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_MAZE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpRegion;

const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *Scene_GetWarpRegions(void);
const u32 *Scene_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
extern struct MapWarpDest gSolSanctumMazeWarpDests[];
extern u32 gSolSanctumMazeWarps[];
extern struct ObjectEventTemplate gSolSanctumMazeObjectEventTemplates[];
extern struct MapEvent gSolSanctumMazeEvents[];

#endif
