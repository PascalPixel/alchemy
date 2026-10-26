#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_LUNA_SOL_ROOMS_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_LUNA_SOL_ROOMS_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *SolSanctumLunaSolRooms_GetWarpDests(void);
const struct MapWarpRegion *SolSanctumLunaSolRooms_GetRegions(void);
const u32 *SolSanctumLunaSolRooms_GetWarps(void);
const struct ObjectEventTemplate *
SolSanctumLunaSolRooms_GetObjectEventTemplates(void);
const struct MapEvent *SolSanctumLunaSolRooms_GetEvents(void);
s32 SolSanctumLunaSolRooms_Initialize(void);
extern struct MapWarpDest gSolSanctumLunaSolRoomsWarpDests[];
extern u32 gSolSanctumLunaSolRoomsWarps[];
extern struct ObjectEventTemplate gSolSanctumLunaSolRoomsObjectEventTemplates[];
extern struct MapEvent gSolSanctumLunaSolRoomsEvents[];

#endif
