#ifndef ALCHEMY_UNIT_MAP_LOG_ROOMS_C_H
#define ALCHEMY_UNIT_MAP_LOG_ROOMS_C_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *LogRoomsC_GetWarpDests(void);
const struct MapWarpRegion *LogRoomsC_GetRegions(void);
const u32 *LogRoomsC_GetWarps(void);
const struct ObjectEventTemplate *LogRoomsC_GetObjectEventTemplates(void);
const struct MapEvent *LogRoomsC_GetEvents(void);
s32 LogRoomsC_Initialize(void);
extern struct MapWarpDest gLogRoomsC_WarpDests[];
extern u32 gLogRoomsC_Warps[];
extern struct ObjectEventTemplate gLogRoomsC_ObjectEventTemplates[];
extern struct MapEvent gLogRoomsC_Events[];

#endif
