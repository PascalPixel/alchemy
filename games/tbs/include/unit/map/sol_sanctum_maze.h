#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_MAZE_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_MAZE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpRegion;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
extern struct MapWarpDest gSolSanctumMazeWarpDests[];
extern u32 gSolSanctumMazeWarps[];
extern struct ObjectEventTemplate gSolSanctumMazeObjectEventTemplates[];
extern struct MapEvent gSolSanctumMazeEvents[];

#endif
