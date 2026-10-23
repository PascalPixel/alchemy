#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_CORRIDOR_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_CORRIDOR_H

#include "types.h"
#include "map_tables.h"

struct MapWarpRegion;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
extern struct MapWarpDest gSolSanctumCorridorWarpDests[];
extern u32 gSolSanctumCorridorWarps[];
extern struct ObjectEventTemplate gSolSanctumCorridorObjectEventTemplates[];
extern struct MapEvent gSolSanctumCorridorEvents[];

#endif
