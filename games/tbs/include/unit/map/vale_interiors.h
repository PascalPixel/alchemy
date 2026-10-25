#ifndef ALCHEMY_UNIT_MAP_VALE_INTERIORS_H
#define ALCHEMY_UNIT_MAP_VALE_INTERIORS_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *ValeInteriors_GetWarpDests(void);
const struct MapWarpRegion *ValeInteriors_GetRegions(void);
const u32 *ValeInteriors_GetWarps(void);
const struct ObjectEventTemplate *ValeInteriors_GetObjectEventTemplates(void);
const struct MapEvent *ValeInteriors_GetEvents(void);
s32 ValeInteriors_Initialize(void);
extern s32 gValeInteriors_FaceTargetScript[];
extern s32 gValeInteriors_FatherPacingScript[];
extern s32 gValeInteriors_PartyLeavesScript[];
extern s32 gValeInteriors_SleepZScript[];
extern struct MapWarpDest gValeInteriors_WarpDests[];
extern u32 gValeInteriors_Warps[];
extern u32 gValeInteriors_StormWarps[];
extern struct ObjectEventTemplate gValeInteriors_ObjectEventTemplates[];
extern struct ObjectEventTemplate
    gValeInteriors_ObjectEventTemplatesAfterLeaving[];
extern struct ObjectEventTemplate
    gValeInteriors_ObjectEventTemplatesAfterKalay[];
extern struct MapEvent gValeInteriors_StormEvents[];
extern struct MapEvent gValeInteriors_Events[];
extern struct MapEvent gValeInteriors_EventsAfterLeaving[];
extern struct MapEvent gValeInteriors_EventsAfterKalay[];

#endif
