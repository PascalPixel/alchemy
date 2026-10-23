#ifndef ALCHEMY_UNIT_MAP_VALE_SHOPS_H
#define ALCHEMY_UNIT_MAP_VALE_SHOPS_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *ValeShops_GetWarpDests(void);
const struct MapWarpRegion *ValeShops_GetRegions(void);
const u32 *ValeShops_GetWarps(void);
const struct ObjectEventTemplate *ValeShops_GetObjectEventTemplates(void);
const struct MapEvent *ValeShops_GetEvents(void);
s32 ValeShops_Initialize(void);
extern s32 gValeShops_FaceTargetScript[];
extern s32 gValeShops_FatherPacingScript[];
extern s32 gValeShops_PartyLeavesScript[];
extern s32 gValeShops_SleepZScript[];
extern struct MapWarpDest gValeShops_WarpDests[];
extern u32 gValeShops_Warps[];
extern u32 gValeShops_StormWarps[];
extern struct ObjectEventTemplate gValeShops_ObjectEventTemplates[];
extern struct ObjectEventTemplate gValeShops_ObjectEventTemplatesAfterLeaving[];
extern struct ObjectEventTemplate gValeShops_ObjectEventTemplatesAfterKalay[];
extern struct MapEvent gValeShops_StormEvents[];
extern struct MapEvent gValeShops_Events[];
extern struct MapEvent gValeShops_EventsAfterLeaving[];
extern struct MapEvent gValeShops_EventsAfterKalay[];

#endif
