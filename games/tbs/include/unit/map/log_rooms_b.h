#ifndef ALCHEMY_UNIT_MAP_LOG_ROOMS_B_H
#define ALCHEMY_UNIT_MAP_LOG_ROOMS_B_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern struct MapWarpDest gLogRoomsB_WarpDests[];
extern u32 gLogRoomsB_Warps[];
extern struct ObjectEventTemplate gLogRoomsB_ObjectEventTemplates[];
extern struct MapEvent gLogRoomsB_Events[];

#endif
