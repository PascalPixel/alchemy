#ifndef ALCHEMY_UNIT_MAP_COTTAGE_ROOMS_C_H
#define ALCHEMY_UNIT_MAP_COTTAGE_ROOMS_C_H

#include "types.h"
#include "map_tables.h"

struct MapWarpRegion;

const struct MapWarpDest *CottageRoomsC_GetWarpDests(void);
const struct MapWarpRegion *CottageRoomsC_GetRegions(void);
const u32 *CottageRoomsC_GetWarps(void);
const struct ObjectEventTemplate *CottageRoomsC_GetObjectEventTemplates(void);
const struct MapEvent *CottageRoomsC_GetEvents(void);
s32 CottageRoomsC_Initialize(void);
extern struct MapWarpDest gCottageRoomsCWarpDests[];
extern u32 gCottageRoomsCWarps[];
extern struct ObjectEventTemplate gCottageRoomsCObjectEventTemplates[];
extern struct MapEvent gCottageRoomsCEvents[];

#endif
