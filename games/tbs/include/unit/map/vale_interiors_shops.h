#ifndef ALCHEMY_UNIT_MAP_VALE_INTERIORS_SHOPS_H
#define ALCHEMY_UNIT_MAP_VALE_INTERIORS_SHOPS_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *ValeInteriorsShops_GetWarpDests(void);
const struct MapWarpRegion *ValeInteriorsShops_GetRegions(void);
const u32 *ValeInteriorsShops_GetWarps(void);
const struct ObjectEventTemplate *
ValeInteriorsShops_GetObjectEventTemplates(void);
const struct MapEvent *ValeInteriorsShops_GetEvents(void);
s32 ValeInteriorsShops_Initialize(void);
extern s32 gValeInteriorsShops_FaceTargetScript[];
extern s32 gValeInteriorsShops_FatherPacingScript[];
extern s32 gValeInteriorsShops_PartyLeavesScript[];
extern s32 gValeInteriorsShops_SleepZScript[];
extern struct MapWarpDest gValeInteriorsShops_WarpDests[];
extern u32 gValeInteriorsShops_Warps[];
extern u32 gValeInteriorsShops_StormWarps[];
extern struct ObjectEventTemplate gValeInteriorsShops_ObjectEventTemplates[];
extern struct ObjectEventTemplate
    gValeInteriorsShops_ObjectEventTemplatesAfterLeaving[];
extern struct ObjectEventTemplate
    gValeInteriorsShops_ObjectEventTemplatesAfterKalay[];
extern struct MapEvent gValeInteriorsShops_StormEvents[];
extern struct MapEvent gValeInteriorsShops_Events[];
extern struct MapEvent gValeInteriorsShops_EventsAfterLeaving[];
extern struct MapEvent gValeInteriorsShops_EventsAfterKalay[];

#endif
