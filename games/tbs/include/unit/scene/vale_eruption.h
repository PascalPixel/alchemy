#ifndef ALCHEMY_UNIT_MAP_VALE_ERUPTION_H
#define ALCHEMY_UNIT_MAP_VALE_ERUPTION_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *ValeEruption_GetWarpDests(void);
const struct MapWarpRegion *ValeEruption_GetRegions(void);
const u32 *ValeEruption_GetWarps(void);
const struct ObjectEventTemplate *ValeEruption_GetObjectEventTemplates(void);
const struct MapEvent *ValeEruption_GetEvents(void);
s32 ValeEruption_Initialize(void);
extern s32 gValeEruption_LeaderWalksDown[];
extern s32 gValeEruption_GaretWalksDown[];
extern s32 gValeEruption_EnterSanctum[];
extern struct MapWarpDest gValeEruption_WarpDests[];
extern u32 gValeEruption_Warps[];
extern struct ObjectEventTemplate gValeEruption_ObjectEventTemplates[];
extern struct MapEvent gValeEruption_Events[];
extern u16 gValeEruption_SanctumDoorOpens[];

#endif
