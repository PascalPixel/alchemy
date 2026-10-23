#ifndef ALCHEMY_UNIT_MAP_KOLIMA_FOREST_GLADE_H
#define ALCHEMY_UNIT_MAP_KOLIMA_FOREST_GLADE_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *KolimaForestGlade_GetWarpDests(void);
const struct MapWarpRegion *KolimaForestGlade_GetRegions(void);
const u32 *KolimaForestGlade_GetWarps(void);
const struct ObjectEventTemplate *
KolimaForestGlade_GetObjectEventTemplates(void);
const struct MapEvent *KolimaForestGlade_GetEvents(void);
s32 KolimaForestGlade_Initialize(void);
extern s32 gStagedObjectEvent_DirectionSteps[];
extern u32 gStagedObjectEvent_FootprintKinds[];
extern s32 gStagedObjectEvent_FootprintBounds[];
extern struct MapWarpDest gKolimaForestGlade_WarpDests[];
extern u32 gKolimaForestGlade_Warps[];
extern struct ObjectEventTemplate gKolimaForestGlade_ObjectEventTemplates[];
extern struct MapEvent gKolimaForestGlade_Events[];

#endif
