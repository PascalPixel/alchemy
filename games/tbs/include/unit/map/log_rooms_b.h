#ifndef ALCHEMY_UNIT_MAP_LOG_ROOMS_B_H
#define ALCHEMY_UNIT_MAP_LOG_ROOMS_B_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *Scene_GetWarpRegions(void);
const u32 *Scene_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct MapWarpDest gLogRoomsB_WarpDests[];
extern u32 gLogRoomsB_Warps[];
extern struct ObjectEventTemplate gLogRoomsB_ObjectEventTemplates[];
extern struct MapEvent gLogRoomsB_Events[];

#endif
